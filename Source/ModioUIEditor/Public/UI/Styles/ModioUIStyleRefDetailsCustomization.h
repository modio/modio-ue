/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#pragma once

#include "Engine/Engine.h"
#include "ModioUISubsystem.h"

class FModioUIStyleRefDetailsCustomization : public IPropertyTypeCustomization
{
	UModioUIStyleSet* StyleBeingEdited;
	TArray<TSharedPtr<FName>> StyleNames;
	TSharedPtr<IPropertyHandle> UnderlyingStyleRefPropHandle;

	FName GetOwningStyleName()
	{
		FStructProperty* UnderlyingStruct = CastField<FStructProperty>(UnderlyingStyleRefPropHandle->GetProperty());

		TArray<UObject*> OuterObjects;

		UnderlyingStyleRefPropHandle->GetOuterObjects(OuterObjects);
		/*if (UnderlyingStruct)
		{
			void* DataPointer;
			PropertyHandle->GetValueData(DataPointer);
			if (DataPointer)
			{
				FModioUIStyleRef* ActualStyleRef = static_cast<FModioUIStyleRef*>(DataPointer);
				return ActualStyleRef->GetStyleName();
			}
		}*/

		// need to look back up along the property tree to see if we find a FSlateWidgetStyle subclass
		return FName(NAME_None);
	}

public:
	void UpdateStyleNameList()
	{
		StyleNames.Empty();
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			UModioUIStyleSet* DefaultStyle = Subsystem->GetDefaultStyleSet();
			if (DefaultStyle)
			{
				TArray<FName> RawStyleNames;

				if (UnderlyingStyleRefPropHandle->HasMetaData("StyleClass"))
				{
					FString StyleClassContainerName = UnderlyingStyleRefPropHandle->GetMetaData("StyleClass");
					UClass* FoundObject = FindObject<UClass>(ANY_PACKAGE, *StyleClassContainerName);
					if (!FoundObject)
					{
						StyleClassContainerName += "Container";
						FoundObject = FindObject<UClass>(ANY_PACKAGE, *StyleClassContainerName);
					}
					if (FoundObject)
					{
						for (const TPair<FName, USlateWidgetStyleContainerBase*> StyleEntry :
							 DefaultStyle->WidgetStyles)
						{
							if (StyleEntry.Value->GetClass()->IsChildOf(FoundObject))
							{
								RawStyleNames.Add(StyleEntry.Key);
							}
						}
					}
				}
				else
				{
					DefaultStyle->WidgetStyles.GetKeys(RawStyleNames);
				}

				FStructProperty* UnderlyingStruct =
					CastField<FStructProperty>(UnderlyingStyleRefPropHandle->GetProperty());

				TArray<UObject*> OuterObjects;

				UnderlyingStyleRefPropHandle->GetOuterObjects(OuterObjects);

				for (UObject* Outer : OuterObjects)
				{
					UModioUIWidgetStyleContainer* OuterContainer = Cast<UModioUIWidgetStyleContainer>(Outer);
					if (OuterContainer)
					{
						const FName* FoundName = DefaultStyle->WidgetStyles.FindKey(OuterContainer);
						if (FoundName)
						{
							RawStyleNames.Remove(*FoundName);
						}
					}
				}
				Algo::Transform(RawStyleNames, StyleNames, [](const FName& Name) { return MakeShared<FName>(Name); });
			}
		}
	}
	void UpdateEditedStyleRef(TSharedPtr<FName> NewName, ESelectInfo::Type SelectionType)
	{
		if (NewName == nullptr)
		{
			return;
		}

		FStructProperty* UnderlyingStruct = CastField<FStructProperty>(UnderlyingStyleRefPropHandle->GetProperty());

		if (UnderlyingStruct)
		{
			void* DataPointer;
			UnderlyingStyleRefPropHandle->NotifyPreChange();
			UnderlyingStyleRefPropHandle->GetValueData(DataPointer);
			if (DataPointer)
			{
				FModioUIStyleRef* ActualStyleRef = static_cast<FModioUIStyleRef*>(DataPointer);
				ActualStyleRef->SetStyleName(*NewName);
				UnderlyingStyleRefPropHandle->NotifyPostChange(EPropertyChangeType::ValueSet);
			}
		}
	}
	FText GetUnderlyingStyleRefName()const
	{
		void* DataPointer;
		UnderlyingStyleRefPropHandle->GetValueData(DataPointer);
		FModioUIStyleRef* ActualStyleRef = static_cast<FModioUIStyleRef*>(DataPointer);
		return FText::FromName(ActualStyleRef ? ActualStyleRef->GetStyleName() : FName("Invalid Object!"));
	}
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow,
								 IPropertyTypeCustomizationUtils& CustomizationUtils) override
	{
		UnderlyingStyleRefPropHandle = PropertyHandle;
		HeaderRow.NameContent()[PropertyHandle->CreatePropertyNameWidget()];

		FStructProperty* UnderlyingStruct = CastField<FStructProperty>(PropertyHandle->GetProperty());

		if (UnderlyingStruct)
		{
			
			// clang-format off
			HeaderRow.ValueContent()
				.MinDesiredWidth(250) // to match SPropertyEditorEditInline
				.MaxDesiredWidth(600)
				[
					SNew(SHorizontalBox)
					+SHorizontalBox::Slot().AutoWidth()
					[
						SNew(SEditableTextBox)
						.Text(this, &FModioUIStyleRefDetailsCustomization::GetUnderlyingStyleRefName)
						.IsReadOnly(true)
					]
					+SHorizontalBox::Slot().AutoWidth()
					[
						SNew(SComboBox<TSharedPtr<FName>>)
						.Content()
						[
							SNew(STextBlock).Text(FText::FromString("Styles"))
						]
						.OnComboBoxOpening(FOnComboBoxOpening::CreateSP(this, &FModioUIStyleRefDetailsCustomization::UpdateStyleNameList))
						.OptionsSource(&StyleNames)
						.OnSelectionChanged(SComboBox<TSharedPtr<FName>>::FOnSelectionChanged::CreateSP(this, &FModioUIStyleRefDetailsCustomization::UpdateEditedStyleRef))
						.OnGenerateWidget(SComboBox<TSharedPtr<FName>>::FOnGenerateWidget::CreateLambda([](TSharedPtr<FName> Name){ return SNew(STextBlock).Text(Name.IsValid()? FText::FromName(*Name) : FText());}))
					]
				];
			// clang-format on
		}
		else
		{
			HeaderRow.ValueContent()[SNew(SColorBlock)];
		}
	}

	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder,
								   IPropertyTypeCustomizationUtils& CustomizationUtils) override {

	};
};
