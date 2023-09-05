/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Foundation/Components/Text/MultiLineEditableTextBox/ModioCommonMultiLineEditableTextBox.h"

#include "UI/Foundation/Components/Text/EditableTextBox/ModioCommonEditableTextBox.h"
#include "UI/Foundation/Components/Text/MultiLineEditableTextBox/ModioCommonMultiLineEditableTextBoxStyle.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"
#include "Widgets/Layout/SGridPanel.h"
#include "Widgets/Layout/SScaleBox.h"
#include "Styling/SlateTypes.h"

void UModioCommonMultiLineEditableTextBox::SetStyle(TSubclassOf<UModioCommonMultiLineEditableTextBoxStyle> InStyle)
{
	ModioStyle = InStyle;
	SynchronizeProperties();
}

bool UModioCommonMultiLineEditableTextBox::IsTextBoxFocused() const
{
	return HasAnyUserFocus() || HasFocusedDescendants();
}

void UModioCommonMultiLineEditableTextBox::SynchronizeProperties()
{
	if (UModioCommonMultiLineEditableTextBoxStyle* StyleCDO = ModioStyle.GetDefaultObject())
	{
		SetHintText(StyleCDO->HintText);
		WidgetStyle.BackgroundImageNormal = StyleCDO->NormalBackgroundBrush;
		WidgetStyle.BackgroundImageHovered = StyleCDO->HoveredBackgroundBrush;
		WidgetStyle.BackgroundImageFocused = StyleCDO->FocusedBackgroundBrush;
		WidgetStyle.BackgroundImageReadOnly = StyleCDO->ReadOnlyBackgroundBrush;
		//WidgetStyle.Padding = StyleCDO->Padding;
		SetTextStyle(StyleCDO->TextStyle);
		WidgetStyle.ForegroundColor = StyleCDO->ForegroundColor;
		WidgetStyle.BackgroundColor = StyleCDO->BackgroundColor;
		WidgetStyle.ReadOnlyForegroundColor = StyleCDO->ReadOnlyForegroundColor;
		WidgetStyle.FocusedForegroundColor = StyleCDO->FocusedForegroundColor;
	}
	Super::SynchronizeProperties();
	if (UModioCommonMultiLineEditableTextBoxStyle* StyleCDO = ModioStyle.GetDefaultObject())
	{
		SetHintText(StyleCDO->HintText);
	}
}

TSharedRef<SWidget> UModioCommonMultiLineEditableTextBox::RebuildWidget()
{
	UModioCommonMultiLineEditableTextBoxStyle* StyleCDO = ModioStyle.GetDefaultObject();
	if (!StyleCDO)
	{
		return Super::RebuildWidget();
	}

	MyEditableTextBlock = SNew(SModioCommonMultiLineEditableTextBox)
		.Style(&WidgetStyle)
		.IsCaretMovedWhenGainFocus(IsCaretMovedWhenGainFocus)
		.SelectAllTextWhenFocused(SelectAllTextWhenFocused)
		.RevertTextOnEscape(RevertTextOnEscape)
		.ClearKeyboardFocusOnCommit(ClearKeyboardFocusOnCommit)
		.SelectAllTextOnCommit(SelectAllTextOnCommit)
		.AllowContextMenu(AllowContextMenu)
		.OnTextChanged(BIND_UOBJECT_DELEGATE(FOnTextChanged, HandleOnTextChanged))
		.OnTextCommitted(BIND_UOBJECT_DELEGATE(FOnTextCommitted, HandleOnTextCommitted))
		//.VirtualKeyboardType(EVirtualKeyboardType::AsKeyboardType(KeyboardType.GetValue()))
		.VirtualKeyboardOptions(VirtualKeyboardOptions)
		.VirtualKeyboardTrigger(VirtualKeyboardTrigger)
		.VirtualKeyboardDismissAction(VirtualKeyboardDismissAction)
		.Justification(Justification)
		.OverflowPolicy(OverflowPolicy)
		.AlwaysShowScrollbars(AlwaysShowScrollbars)
		.AllowMultiLine(AllowMultiLine);
	
	// clang-format off
	return SAssignNew(MyVerticalBox, SModioCommonFocusableVerticalBox)
	       + SVerticalBox::Slot()
	         .FillHeight(1.0f)
	         .HAlign(HAlign_Fill)
	         .VAlign(VAlign_Fill)
	         .SizeParam(FStretch(1.0f))
	       [
		       SNew(SBorder).HAlign(HAlign_Fill).VAlign(VAlign_Fill)
		       .BorderImage_UObject(this, &UModioCommonMultiLineEditableTextBox::GetBorderImage)
		       [
			       SNew(SGridPanel)
			       .FillColumn(1, 1)
			       .FillColumn(1, 1)
			       .FillRow(0, 1)
			       + SGridPanel::Slot(0, 0)
			       .Padding(StyleCDO->HintIconPadding)
			       [
				       SNew(SScaleBox)
				       .Stretch(EStretch::ScaleToFitX)
				       .StretchDirection(EStretchDirection::Both)
				       .VAlign(VAlign_Top)
				       [
					       SNew(SImage)
					       .Image_UObject(this, &UModioCommonMultiLineEditableTextBox::GetHintBrush)
					       .Visibility_UObject(this, &UModioCommonMultiLineEditableTextBox::GetHintBrushVisibility)
				       ]
			       ]
			       + SGridPanel::Slot(1, 0)
			       [
				       MyEditableTextBlock.ToSharedRef()
			       ]
		       ]
	       ];
	// clang-format on
}

void UModioCommonMultiLineEditableTextBox::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	MyVerticalBox.Reset();
}

const FSlateBrush* UModioCommonMultiLineEditableTextBox::GetHintBrush() const
{
	UModioCommonMultiLineEditableTextBoxStyle* StyleCDO = ModioStyle.GetDefaultObject();
	if (!StyleCDO)
	{
		return nullptr;
	}
	return &StyleCDO->HintIcon;
}

EVisibility UModioCommonMultiLineEditableTextBox::GetHintBrushVisibility() const
{
	if (const UModioCommonMultiLineEditableTextBoxStyle* StyleCDO = ModioStyle.GetDefaultObject())
	{
		return StyleCDO->bShowHintIcon ? EVisibility::Visible : EVisibility::Collapsed;
	}
	return EVisibility::Collapsed;
}

const FSlateBrush* UModioCommonMultiLineEditableTextBox::GetBorderImage() const
{
	if (const UModioCommonMultiLineEditableTextBoxStyle* StyleCDO = ModioStyle.GetDefaultObject())
	{
		if (HasValidationError())
		{
			return &StyleCDO->ErrorBorderBrush;
		}
		if (IsHovered() || HasAnyUserFocus() || HasFocusedDescendants())
		{
			return &StyleCDO->HoveredBorderBrush;
		}
		return &StyleCDO->NormalBorderBrush;
	}
	return nullptr;
}

void UModioCommonMultiLineEditableTextBox::HandleOnTextChanged(const FText& InText)
{
	if (const UModioCommonMultiLineEditableTextBoxStyle* StyleCDO = ModioStyle.GetDefaultObject())
	{
		if (StyleCDO->MaxTextLength > 0 && InText.ToString().Len() > StyleCDO->MaxTextLength)
		{
			const FString TruncatedString = InText.ToString().Left(StyleCDO->MaxTextLength);
			SetText(FText::FromString(TruncatedString));
			return;
		}
	}
	Super::HandleOnTextChanged(InText);
	OnMultiLineEditableTextChanged.Broadcast(InText);
}
