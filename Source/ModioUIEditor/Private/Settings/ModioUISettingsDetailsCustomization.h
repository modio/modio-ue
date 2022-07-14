#pragma once

#include "Algo/Transform.h"
#include "Containers/UnrealString.h"
#include "GameFramework/InputSettings.h"
#include "IDetailChildrenBuilder.h"
#include "PropertyCustomizationHelpers.h"
#include "PropertyEditor/Public/DetailLayoutBuilder.h"
#include "PropertyEditor/Public/DetailWidgetRow.h"
#include "PropertyEditor/Public/IPropertyTypeCustomization.h"
#include "PropertyEditor/Public/PropertyHandle.h"
#include "UI/Interfaces/IModioInputMappingAccessor.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Input/SEditableTextBox.h"

class FModioFixedWidthArrayDetailBuilder : public FDetailArrayBuilder
{
public:
	using FDetailArrayBuilder::FDetailArrayBuilder;
	virtual void GenerateHeaderRowContent(FDetailWidgetRow& NodeRow) override
	{
		FDetailArrayBuilder::GenerateHeaderRowContent(NodeRow);
		NodeRow.ValueContent().MinDesiredWidth(250).MaxDesiredWidth(600);
	}
};

class FModioInputMappingCustomization : public IPropertyTypeCustomization
{
public:
	void CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, class FDetailWidgetRow& HeaderRow,
						 IPropertyTypeCustomizationUtils& StructCustomizationUtils)
	{
		HeaderRow.NameContent()[StructPropertyHandle->CreatePropertyNameWidget()];

		TSharedPtr<IPropertyHandle> KeyProp = StructPropertyHandle->GetChildHandle(FName("VirtualKey"));

		if (KeyProp)
		{
			TArray<void*> KeyData;
			KeyProp->AccessRawData(KeyData);
			if (KeyData.Num())
			{
				FKey* ActualKey = static_cast<FKey*>(KeyData[0]);
				HeaderRow.ValueContent()
					.HAlign(HAlign_Left)
					.MinDesiredWidth(250)
					.MaxDesiredWidth(600)[SNew(SEditableTextBox).IsEnabled(false).Text(ActualKey->GetDisplayName())];
			}
		}
	}
	void GetValuesForMapping(TArray<TSharedPtr<FString>>& ValidValuesForMapping, TArray<TSharedPtr<SToolTip>>&,
							 TArray<bool>&)
	{
		UInputSettings* CurrentInputSettings = UInputSettings::GetInputSettings();
		if (!CurrentInputSettings)
		{
			ValidValuesForMapping.Add(MakeShared<FString>("Could not get Input Settings!"));
			return;
		}
		ValidValuesForMapping.Add(MakeShared<FString>("None"));
		TArray<FName> Names;
		CurrentInputSettings->GetAxisNames(Names);

		Algo::Transform(Names, ValidValuesForMapping,
						[](FName CurrentName) { return MakeShared<FString>(CurrentName.ToString()); });
		CurrentInputSettings->GetActionNames(Names);
		Algo::Transform(Names, ValidValuesForMapping,
						[](FName CurrentName) { return MakeShared<FString>(CurrentName.ToString()); });
	}
	void OnGenerateElementForMappedInput(TSharedRef<IPropertyHandle> ElementHandle, int32,
										 IDetailChildrenBuilder& ChildBuilder)
	{
		FDetailWidgetRow& NewRow = ChildBuilder.AddCustomRow(FText::FromString(""));
		NewRow.NameContent()[ElementHandle->CreatePropertyNameWidget()];
		NewRow.ValueContent().MinDesiredWidth(250).MaxDesiredWidth(
			600)[PropertyCustomizationHelpers::MakePropertyComboBox(
			ElementHandle,
			FOnGetPropertyComboBoxStrings::CreateSP(this, &FModioInputMappingCustomization::GetValuesForMapping))];
	}
	void CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle,
						   class IDetailChildrenBuilder& StructBuilder,
						   IPropertyTypeCustomizationUtils& StructCustomizationUtils)
	{
		if (StructPropertyHandle->IsValidHandle())
		{
			if (TSharedPtr<IPropertyHandle> MappedInputsProp =
					StructPropertyHandle->GetChildHandle(FName("MappedProjectInputs")))
			{
				TSharedRef<FModioFixedWidthArrayDetailBuilder> InputArrayBuilder =
					MakeShareable(new FModioFixedWidthArrayDetailBuilder(MappedInputsProp.ToSharedRef()));
				InputArrayBuilder->OnGenerateArrayElementWidget(FOnGenerateArrayElementWidget::CreateSP(
					this, &FModioInputMappingCustomization::OnGenerateElementForMappedInput));
				StructBuilder.AddCustomBuilder(InputArrayBuilder);
			}
		}
	}
};
