#pragma once

#include "Algo/Transform.h"
#include "Containers/Array.h"
#include "Containers/Map.h"
#include "Containers/UnrealString.h"
#include "Editor/ModioDetailChildrenBuilder_EditConditionHook.h"
#include "Editor/ModioDetailCustomNodeOverrideBuilder.h"
#include "PropertyEditor/Public/DetailLayoutBuilder.h"
#include "PropertyEditor/Public/DetailWidgetRow.h"
#include "PropertyEditor/Public/IDetailChildrenBuilder.h"
#include "PropertyEditor/Public/IDetailCustomization.h"
#include "PropertyEditor/Public/PropertyHandle.h"
#include "PropertyPath.h"
#include "PropertyPathHelpers.h"
#include "UObject/NoExportTypes.h"
#include "UObject/UnrealNames.h"

class FModioTextBlockStyleOverrideDetailsCustomization : public IPropertyTypeCustomization
{
public:
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow,
								 IPropertyTypeCustomizationUtils& CustomizationUtils) override
	{
		HeaderRow.NameContent()[PropertyHandle->CreatePropertyNameWidget()];
		HeaderRow.ValueContent()[PropertyHandle->CreatePropertyValueWidget(false)];
	}
	FName GetRelativePropertyPath(TSharedPtr<IPropertyHandle> ParentHandle, TSharedPtr<IPropertyHandle> PropertyHandle) const
	{
		FCachedPropertyPath ParentPath = ParentHandle->GeneratePathToProperty();
		FCachedPropertyPath ChildPath = PropertyHandle->GeneratePathToProperty();
		while (ParentPath.GetNumSegments() && ParentPath.GetSegment(0).GetName() == ChildPath.GetSegment(0).GetName())
		{
			ParentPath.RemoveFromStart();
			ChildPath.RemoveFromStart();
		}
		return FName(ChildPath.ToString());

	}
	void OnOverrideToggled(bool NewValue, TSharedPtr<IPropertyHandle> PropertyWithOverrideToggled)
	{
		FName Path = GetRelativePropertyPath(StylePropertyHandle, PropertyWithOverrideToggled);
		if (Path != NAME_None)
		{
			if (OverrideStatusMap)
			{
				OverrideStatusMap->FindOrAdd(Path) = NewValue;
			}
		}
	}
	bool IsOverrideEnabled(TSharedPtr<IPropertyHandle> PropertyToCheck) const
	{
		FName Path = GetRelativePropertyPath(StylePropertyHandle, PropertyToCheck);
		if (Path != NAME_None)
		{
			if (OverrideStatusMap)
			{
				if (OverrideStatusMap->Contains(Path))
				{
					return *OverrideStatusMap->Find(Path);
				}
			}
		}
		return false;
	}
	TMap<FName, bool>* OverrideStatusMap = nullptr;
	TSharedPtr<FModioDetailChildrenBuilder_EditConditionHook> BuilderWrapper;
	TSharedPtr<FModioDetailCustomNodeOverrideBuilder> CustomBuilder;

	TSharedPtr<IPropertyHandle> StylePropertyHandle;

	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder,
								   IPropertyTypeCustomizationUtils& CustomizationUtils) override
	{
		uint32 NumChildProperties = 0;

		TSharedPtr<IPropertyHandle> OverrideMapHandle = PropertyHandle->GetChildHandle(FName("OverriddenProperties"));
		if (OverrideMapHandle)
		{
			TArray<void*> ObjectRawData;
			OverrideMapHandle->AccessRawData(ObjectRawData);
			if (ObjectRawData.Num())
			{
				OverrideStatusMap = reinterpret_cast<TMap<FName, bool>*>(ObjectRawData[0]);
			}
			// Don't need to display this property as it's just storage for the override flag
			// ChildBuilder.AddProperty(OverrideMapHandle.ToSharedRef());
		}
		BuilderWrapper = MakeShared<FModioDetailChildrenBuilder_EditConditionHook>(
			FGetEditConditionValue::CreateSP(this,
											 &FModioTextBlockStyleOverrideDetailsCustomization::IsOverrideEnabled),
			FUpdateEditConditionValue::CreateSP(this,
												&FModioTextBlockStyleOverrideDetailsCustomization::OnOverrideToggled),
			ChildBuilder);

		StylePropertyHandle = PropertyHandle->GetChildHandle(FName("StyleProperties"));
		//Perhaps store this and use it to trim the property path before we save it out?



		/*if (StylePropertyHandle)
		{
			CustomBuilder =
				MakeShared<FModioDetailCustomNodeOverrideBuilder>(StylePropertyHandle.ToSharedRef(), OverrideStatusMap);

			ChildBuilder.AddCustomBuilder(CustomBuilder.ToSharedRef());
		}*/
		// Implement custom node builder
		// Custom node builder, creates a BuilderWrapper in IDetailCustomNodeBuilder::GenerateChildContent
		// ChildBuilder->AddCustomBuilder(CustomNodeBuilder(StylePropertyHandle, MapPropertyHandle));
		if (StylePropertyHandle)
		{
			StylePropertyHandle->GetNumChildren(NumChildProperties);

			if (NumChildProperties)
			{
				for (uint32 CurrentChildPropIndex = 0; CurrentChildPropIndex < NumChildProperties;
					 CurrentChildPropIndex++)
				{
					TSharedPtr<IPropertyHandle> CurrentPropertyHandle =
						StylePropertyHandle->GetChildHandle(CurrentChildPropIndex);
					if (CurrentPropertyHandle)
					{
						IDetailPropertyRow& PropertyRow =
							BuilderWrapper->AddProperty(CurrentPropertyHandle.ToSharedRef());
					}
				}
			}
		}
	};
};