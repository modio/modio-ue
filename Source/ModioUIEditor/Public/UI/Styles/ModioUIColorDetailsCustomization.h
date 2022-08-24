#pragma once

#include "Algo/Transform.h"
#include "Containers/Array.h"
#include "Containers/UnrealString.h"
#include "Core/ModioUIHelpers.h"
#include "PropertyEditor/Public/DetailLayoutBuilder.h"
#include "PropertyEditor/Public/DetailWidgetRow.h"
#include "PropertyEditor/Public/IDetailChildrenBuilder.h"
#include "PropertyEditor/Public/IDetailCustomization.h"
#include "PropertyEditor/Public/IPropertyUtilities.h"
#include "PropertyEditor/Public/PropertyHandle.h"
#include "UI/Styles/ModioUIColorRef.h"
#include "UI/Styles/ModioUIStyleSet.h"
#include "UObject/UnrealNames.h"
#include "Widgets/Colors/SColorBlock.h"
#include "Widgets/Colors/SColorPicker.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"

class FModioEditorWidgets
{
public:
	static TSharedRef<SWidget> GenerateColorPresetChoiceWidget(TSharedPtr<FName> ColorName,
															   UModioUIStyleSet* StyleToSearch)
	{
		if (StyleToSearch)
		{
			if (StyleToSearch->ColorPresets.Contains(*ColorName.Get()))
			{
				FModioUIColor& PreviewColor = StyleToSearch->ColorPresets[*ColorName.Get()];
				return SNew(SHorizontalBox) +
					   SHorizontalBox::Slot()
						   .HAlign(HAlign_Fill)
						   .VAlign(VAlign_Fill)[SNew(SImage).Image(PreviewColor.GetBrush())] +
					   SHorizontalBox::Slot()
						   .HAlign(HAlign_Fill)
						   .VAlign(VAlign_Fill)
						   .AutoWidth()[SNew(STextBlock).Text(FText::FromName(*ColorName.Get()))];
			}
		}
		return SNullWidget::NullWidget;
	}

	template<typename SourceType>
	static FLinearColor GetUnderlyingColor(TSharedRef<IPropertyHandle> PropertyHandle)
	{}

	template<typename SourceType>
	static FReply ShowColorPicker(TSharedRef<IPropertyHandle> PropertyHandle)
	{}
};

template<>
FLinearColor FModioEditorWidgets::GetUnderlyingColor<FModioUIColor>(TSharedRef<IPropertyHandle> PropertyHandle)
{
	FStructProperty* UnderlyingStruct = CastField<FStructProperty>(PropertyHandle->GetProperty());

	if (UnderlyingStruct)
	{
		void* DataPointer = nullptr;
		PropertyHandle->GetValueData(DataPointer);
		if (DataPointer)
		{
			FModioUIColor* ActualColor = static_cast<FModioUIColor*>(DataPointer);
			return (FLinearColor) *ActualColor;
		}
	}

	{
		return FLinearColor(0.25, 0.25, 0.25, 1);
	}
}

template<>
FLinearColor FModioEditorWidgets::GetUnderlyingColor<FModioUIColorRef>(TSharedRef<IPropertyHandle> PropertyHandle)
{
	FStructProperty* UnderlyingStruct = CastField<FStructProperty>(PropertyHandle->GetProperty());

	if (UnderlyingStruct)
	{
		void* DataPointer;
		PropertyHandle->GetValueData(DataPointer);
		if (DataPointer)
		{
			FModioUIColorRef* ActualColorReference = static_cast<FModioUIColorRef*>(DataPointer);
			return ActualColorReference->ResolveReference().GetSpecifiedColor();
		}
	}
	return FLinearColor(0.25, 0.25, 0.25, 1);
}

class FModioUIColorDetailsCustomization : public IPropertyTypeCustomization
{
	TSharedPtr<IPropertyHandle> UnderlyingSlateColorPropHandle;

public:
	TArray<FLinearColor*> CurrentlyEditedColors;

	virtual FReply OnMouseButtonDownColorBlock(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent,
											   TSharedRef<IPropertyHandle> PropertyHandle)
	{
		CurrentlyEditedColors.Empty();
		void* DataPointer = nullptr;
		PropertyHandle->GetValueData(DataPointer);
		if (DataPointer)
		{
			FModioUIColor* ActualColor = static_cast<FModioUIColor*>(DataPointer);

			CurrentlyEditedColors.Add(ActualColor->GetUnderlyingMutableColor());

			FColorPickerArgs Args;
			Args.bIsModal = true;
			Args.bUseAlpha = true;
			Args.LinearColorArray = &CurrentlyEditedColors;
			OpenColorPicker(Args);
		}
		return FReply::Handled();
	}

	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow,
								 IPropertyTypeCustomizationUtils& CustomizationUtils) override
	{
		UnderlyingSlateColorPropHandle = PropertyHandle;
		HeaderRow.NameContent()[PropertyHandle->CreatePropertyNameWidget()];

		FStructProperty* UnderlyingStruct = CastField<FStructProperty>(PropertyHandle->GetProperty());

		if (UnderlyingStruct)
		{
			HeaderRow.ValueContent()[SNew(SColorBlock)
										 .Color_Static(&FModioEditorWidgets::GetUnderlyingColor<FModioUIColor>,
													   PropertyHandle)
										 .ShowBackgroundForAlpha(false)
#if UE_VERSION_NEWER_THAN(5, 0, 0)
										 .AlphaDisplayMode(EColorBlockAlphaDisplayMode::Ignore)
#else
										 .IgnoreAlpha(true)
#endif

										 .OnMouseButtonDown(
											 this, &FModioUIColorDetailsCustomization::OnMouseButtonDownColorBlock,
											 PropertyHandle)
										 .Size(FVector2D(35.0f, 12.0f))];
			return;
		}
		else
		{
			HeaderRow.ValueContent()[SNew(SColorBlock)];
		}
	}

	/**
	 * Called when the children of the property should be customized or extra rows added
	 *
	 * @param PropertyHandle			Handle to the property being customized
	 * @param StructBuilder				A builder for adding children
	 * @param StructCustomizationUtils	Utilities for customization
	 */
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder,
								   IPropertyTypeCustomizationUtils& CustomizationUtils) override {

	};
};

class FModioUIColorRefDetailsCustomization : public IPropertyTypeCustomization
{
	UModioUIStyleSet* StyleBeingEdited;
	TArray<TSharedPtr<FName>> ColorPresetNames;
	TSharedPtr<IPropertyHandle> UnderlyingColorRefPropHandle;

public:
	FModioUIColorRefDetailsCustomization()
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		{
			if (Subsystem)
			{
				StyleBeingEdited = Subsystem->GetDefaultStyleSet();
			}
		}
	};

	void UpdateColorPresetList()
	{
		ColorPresetNames.Empty();

		Algo::Transform(StyleBeingEdited->ColorPresets, ColorPresetNames,
						[](const TPair<FName, FModioUIColor>& Preset) { return MakeShared<FName>(Preset.Key); });
	}

	void UpdateUnderlyingColor(TSharedPtr<FName> ColorName, ESelectInfo::Type)
	{
		FStructProperty* UnderlyingStruct = CastField<FStructProperty>(UnderlyingColorRefPropHandle->GetProperty());

		if (ColorName.IsValid() && UnderlyingStruct && StyleBeingEdited->ColorPresets.Contains(*ColorName.Get()))
		{
			UnderlyingColorRefPropHandle->NotifyPreChange();
			void* DataPointer;
			UnderlyingColorRefPropHandle->GetValueData(DataPointer);
			if (DataPointer)
			{
				FModioUIColorRef* ActualColorReference = static_cast<FModioUIColorRef*>(DataPointer);
				ActualColorReference->SetLinkedColor(*ColorName.Get());
				UnderlyingColorRefPropHandle->NotifyPostChange(EPropertyChangeType::ValueSet);
			}
		}
	}
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow,
								 IPropertyTypeCustomizationUtils& CustomizationUtils) override
	{
		if (!StyleBeingEdited)
		{
			HeaderRow.ValueContent()[SNew(STextBlock).Text(FText::FromString("No default style set!"))];
			return;
		}
		UnderlyingColorRefPropHandle = PropertyHandle;
		TSharedPtr<SWidget> NameWidget = PropertyHandle->CreatePropertyNameWidget();
		NameWidget->SetEnabled(false);
		HeaderRow.NameContent()[NameWidget.ToSharedRef()];

		FStructProperty* UnderlyingStruct = CastField<FStructProperty>(PropertyHandle->GetProperty());

		if (UnderlyingStruct)
		{
			// clang-format off
			HeaderRow.ValueContent()
				[
					SNew(SHorizontalBox) 
					+SHorizontalBox::Slot().AutoWidth().MaxWidth(32.f)
					[
						SNew(SColorBlock)
						.Color_Static(FModioEditorWidgets::GetUnderlyingColor<FModioUIColorRef>, PropertyHandle)
					] 
					+SHorizontalBox::Slot().AutoWidth()
					[
						SNew(SComboBox<TSharedPtr<FName>>)
						.Content()
						[
							SNew(STextBlock).Text(FText::FromString("Color Presets"))
						]
						.OnComboBoxOpening(FOnComboBoxOpening::CreateSP(this, &FModioUIColorRefDetailsCustomization::UpdateColorPresetList))
						.OptionsSource(&ColorPresetNames)
						.OnSelectionChanged(SComboBox<TSharedPtr<FName>>::FOnSelectionChanged::CreateSP(this, &FModioUIColorRefDetailsCustomization::UpdateUnderlyingColor))
						.OnGenerateWidget(SComboBox<TSharedPtr<FName>>::FOnGenerateWidget::CreateStatic(&FModioEditorWidgets::GenerateColorPresetChoiceWidget, StyleBeingEdited))
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
