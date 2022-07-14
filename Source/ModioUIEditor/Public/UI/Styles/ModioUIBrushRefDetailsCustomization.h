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
#include "UI/Styles/ModioUIBrushRef.h"
#include "UI/Styles/ModioUIStyleSet.h"

class FModioUIMaterialRefDetailsCustomization : public IPropertyTypeCustomization
{
	UModioUIStyleSet* StyleBeingEdited;
	TArray<TSharedPtr<FName>> MaterialNames;
	TSharedPtr<IPropertyHandle> UnderlyingMaterialRefPropHandle;

public:
	void UpdateMaterialNameList()
	{
		MaterialNames.Empty();
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			UModioUIStyleSet* DefaultStyle = Subsystem->GetDefaultStyleSet();

			if (DefaultStyle)
			{
				TArray<FName> RawMaterialNames = DefaultStyle->GetMaterialNames();

				Algo::Transform(RawMaterialNames, MaterialNames, [](const FName& Name) { return MakeShared<FName>(Name); });
				if (ComboBox)
				{
					ComboBox->RefreshOptions();
				}
			}
		}
	}
	void UpdateEditedMaterialRef(TSharedPtr<FName> NewName, ESelectInfo::Type SelectionType)
	{
		if (NewName == nullptr)
		{
			return;
		}

		FStructProperty* UnderlyingStruct = CastField<FStructProperty>(UnderlyingMaterialRefPropHandle->GetProperty());

		if (UnderlyingStruct)
		{
			void* DataPointer;
			UnderlyingMaterialRefPropHandle->NotifyPreChange();
			UnderlyingMaterialRefPropHandle->GetValueData(DataPointer);
			if (DataPointer)
			{
				FModioUIMaterialRef* ActualMaterialRef
					= static_cast<FModioUIMaterialRef*>(DataPointer);
				ActualMaterialRef->SetMaterialName(*NewName);
				UnderlyingMaterialRefPropHandle->NotifyPostChange(EPropertyChangeType::ValueSet);
			}
		}
	}
	FText GetUnderlyingMaterialRefName() const
	{
		void* DataPointer;
		UnderlyingMaterialRefPropHandle->GetValueData(DataPointer);
		FModioUIMaterialRef* ActualMaterialRef = static_cast<FModioUIMaterialRef*>(DataPointer);
		return FText::FromName(ActualMaterialRef ? ActualMaterialRef->GetMaterialName() : FName("Invalid Object!"));
	}
	TSharedPtr< SComboBox<TSharedPtr<FName>>> ComboBox;
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow,
								 IPropertyTypeCustomizationUtils& CustomizationUtils) override
	{
		UnderlyingMaterialRefPropHandle = PropertyHandle;
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
						.Text(this, &FModioUIMaterialRefDetailsCustomization::GetUnderlyingMaterialRefName)
						.IsReadOnly(true)
					]
					+SHorizontalBox::Slot().AutoWidth()
					[
						SAssignNew(ComboBox, SComboBox<TSharedPtr<FName>>)
						.Content()
						[
							SNew(STextBlock).Text(FText::FromString("Named Materials"))
						]
						.OnComboBoxOpening(FOnComboBoxOpening::CreateSP(this, &FModioUIMaterialRefDetailsCustomization::UpdateMaterialNameList))
						.OptionsSource(&MaterialNames)
						.OnSelectionChanged(SComboBox<TSharedPtr<FName>>::FOnSelectionChanged::CreateSP(this, &FModioUIMaterialRefDetailsCustomization::UpdateEditedMaterialRef))
						.OnGenerateWidget(SComboBox<TSharedPtr<FName>>::FOnGenerateWidget::CreateLambda([](TSharedPtr<FName> Name){ return SNew(STextBlock).Text(Name.IsValid()? FText::FromName(*Name) : FText());}))
					]
				];
			// clang-format on
		}
		else
		{
			HeaderRow.ValueContent()[SNullWidget::NullWidget];
		}
	}

	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder,
								   IPropertyTypeCustomizationUtils& CustomizationUtils) override {

	};
};
