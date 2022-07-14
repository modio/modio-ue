#pragma once
#include "Core/Input/ModioInputKeys.h"
#include "InputCoreTypes.h"
#include "Internationalization/Text.h"
#include "PropertyEditor/Public/DetailLayoutBuilder.h"
#include "PropertyEditor/Public/DetailWidgetRow.h"
#include "PropertyEditor/Public/IDetailChildrenBuilder.h"
#include "PropertyEditor/Public/IDetailCustomization.h"
#include "PropertyEditor/Public/PropertyHandle.h"
#include "SKeySelector.h"
#include "Templates/SharedPointer.h"
#include "Widgets/Text/STextBlock.h"

class FModioKeyReadOnlyDetailsCustomization : public IPropertyTypeCustomization
{
public:
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow,
								 IPropertyTypeCustomizationUtils& CustomizationUtils) override
	{
		FText PropertyLabel = FText::FromString("Unknown Key");
		TArray<void*> PropertyData;
		PropertyHandle->AccessRawData(PropertyData);
		if (PropertyData.Num())
		{
			FKey* UnderlyingKey = static_cast<FKey*>(PropertyData[0]);
			PropertyLabel = UnderlyingKey->GetDisplayName();
		}
		HeaderRow.NameContent()[SNew(STextBlock).Text(PropertyLabel)];
	}
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder,
								   IPropertyTypeCustomizationUtils& CustomizationUtils) override
	{}
};

class FModioKeyEditDetailsCustomization : public IPropertyTypeCustomization
{
	FText GetKeyDisplayName(TSharedRef<IPropertyHandle> PropertyHandle) const
	{
		FText PropertyLabel = FText::FromString("Unknown Key");
		TArray<void*> PropertyData;
		PropertyHandle->AccessRawData(PropertyData);
		if (PropertyData.Num())
		{
			FKey* UnderlyingKey = static_cast<FKey*>(PropertyData[0]);
			PropertyLabel = UnderlyingKey->GetDisplayName();
		}
		return PropertyLabel;
	}

	void UpdateKeyList()
	{
		TArray<FKey> AllKeys;
		EKeys::GetAllKeys(AllKeys);

		KeyList.Empty();
		KeyList.Add(MakeShared<FKey>(EKeys::Invalid));
		Algo::Transform(
			AllKeys.FilterByPredicate([](const FKey& Key) { return Key.GetMenuCategory() == NAME_ModioInputKeys; }),
			KeyList, [](const FKey& Key) { return MakeShared<FKey>(Key); });
	}

	void UpdateEditedKeyDetails(TSharedPtr<FKey> NewKey, ESelectInfo::Type SelectionType,
								TSharedRef<IPropertyHandle> PropertyHandle)
	{
		TArray<void*> PropertyData;
		PropertyHandle->AccessRawData(PropertyData);
		if (PropertyData.Num())
		{
			FKey* UnderlyingKey = static_cast<FKey*>(PropertyData[0]);
			PropertyHandle->NotifyPreChange();
			if (NewKey.IsValid())
			{
				*UnderlyingKey = *NewKey.Get();
			}
			PropertyHandle->NotifyPostChange(EPropertyChangeType::ValueSet);
		}
	}
	TArray<TSharedPtr<FKey>> KeyList;
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow,
								 IPropertyTypeCustomizationUtils& CustomizationUtils) override
	{
		HeaderRow.NameContent()[PropertyHandle->CreatePropertyNameWidget()];
		// clang-format off
		HeaderRow.ValueContent()
		[
			SNew(SComboBox<TSharedPtr<FKey>>)
			.Content()
			[
				SNew(STextBlock).Text(this, &FModioKeyEditDetailsCustomization::GetKeyDisplayName, PropertyHandle)
			]
			.OnComboBoxOpening(FOnComboBoxOpening::CreateSP(this, &FModioKeyEditDetailsCustomization::UpdateKeyList))
			.OptionsSource(&KeyList)
			.OnSelectionChanged(SComboBox<TSharedPtr<FKey>>::FOnSelectionChanged::CreateSP(this, &FModioKeyEditDetailsCustomization::UpdateEditedKeyDetails, PropertyHandle))
			.OnGenerateWidget(SComboBox<TSharedPtr<FKey>>::FOnGenerateWidget::CreateLambda([](TSharedPtr<FKey> Key){ return SNew(STextBlock).Text(Key.IsValid()? Key->GetDisplayName() : FText());}))
		];
		// clang-format on
	}
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder,
								   IPropertyTypeCustomizationUtils& CustomizationUtils) override
	{}
};