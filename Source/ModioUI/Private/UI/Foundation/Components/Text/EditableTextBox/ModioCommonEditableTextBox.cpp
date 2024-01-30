/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Foundation/Components/Text/EditableTextBox/ModioCommonEditableTextBox.h"

#include "UI/Foundation/Components/Text/EditableTextBox/ModioCommonEditableTextBoxStyle.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Layout/SGridPanel.h"
#include "Widgets/Layout/SScaleBox.h"
#include "Misc/EngineVersionComparison.h"
#include "UI/Foundation/Components/Extra/SModioCommonFocusableVerticalBox.h"

void UModioCommonEditableTextBox::SetStyle(TSubclassOf<UModioCommonEditableTextBoxStyle> InStyle)
{
	if (InStyle && InStyle != ModioStyle)
	{
		ModioStyle = InStyle;
		SynchronizeProperties();
	}
}

bool UModioCommonEditableTextBox::IsTextBoxFocused() const
{
	return HasAnyUserFocus() || HasFocusedDescendants();
}

void UModioCommonEditableTextBox::SynchronizeProperties()
{
	if (UModioCommonEditableTextBoxStyle* StyleCDO = ModioStyle.GetDefaultObject())
	{
		SetHintText(StyleCDO->HintText);
		WidgetStyle.BackgroundImageNormal = StyleCDO->NormalBackgroundBrush;
		WidgetStyle.BackgroundImageHovered = StyleCDO->HoveredBackgroundBrush;
		WidgetStyle.BackgroundImageFocused = StyleCDO->FocusedBackgroundBrush;
		WidgetStyle.BackgroundImageReadOnly = StyleCDO->ReadOnlyBackgroundBrush;
		WidgetStyle.Padding = StyleCDO->Padding;
		WidgetStyle.SetFont(StyleCDO->Font);
		WidgetStyle.ForegroundColor = StyleCDO->ForegroundColor;
		WidgetStyle.BackgroundColor = StyleCDO->BackgroundColor;
		WidgetStyle.ReadOnlyForegroundColor = StyleCDO->ReadOnlyForegroundColor;
		WidgetStyle.FocusedForegroundColor = StyleCDO->FocusedForegroundColor;
	}
	Super::SynchronizeProperties();
	if (UModioCommonEditableTextBoxStyle* StyleCDO = ModioStyle.GetDefaultObject())
	{
		SetHintText(StyleCDO->HintText);
	}
}

TSharedRef<SWidget> UModioCommonEditableTextBox::RebuildWidget()
{
	UModioCommonEditableTextBoxStyle* StyleCDO = ModioStyle.GetDefaultObject();
	if (!StyleCDO)
	{
		return Super::RebuildWidget();
	}

	MyEditableTextBlock = SNew(SModioCommonEditableTextBox)
		.Style(&WidgetStyle)
		.MinDesiredWidth(
#if UE_VERSION_NEWER_THAN(5, 1, 0)
			GetMinimumDesiredWidth()
#else
			MinimumDesiredWidth
#endif
		)
		.IsCaretMovedWhenGainFocus(
#if UE_VERSION_NEWER_THAN(5, 1, 0)
			GetIsCaretMovedWhenGainFocus()
#else
			IsCaretMovedWhenGainFocus
#endif
		)
		.SelectAllTextWhenFocused(
#if UE_VERSION_NEWER_THAN(5, 1, 0)
			GetSelectAllTextWhenFocused()
#else
			SelectAllTextWhenFocused
#endif
		)
		.RevertTextOnEscape(
#if UE_VERSION_NEWER_THAN(5, 1, 0)
		GetRevertTextOnEscape()
#else
		RevertTextOnEscape
#endif
		)
		.ClearKeyboardFocusOnCommit(
#if UE_VERSION_NEWER_THAN(5, 1, 0)
		GetClearKeyboardFocusOnCommit()
#else
		ClearKeyboardFocusOnCommit
#endif
		)
		.SelectAllTextOnCommit(
#if UE_VERSION_NEWER_THAN(5, 1, 0)
		GetSelectAllTextOnCommit()
#else
		SelectAllTextOnCommit
#endif
		)
		.AllowContextMenu(AllowContextMenu)
		.OnTextChanged(BIND_UOBJECT_DELEGATE(FOnTextChanged, HandleOnTextChanged))
		.OnTextCommitted(BIND_UOBJECT_DELEGATE(FOnTextCommitted, HandleOnTextCommitted))
		.VirtualKeyboardType(EVirtualKeyboardType::AsKeyboardType(KeyboardType.GetValue()))
		.VirtualKeyboardOptions(VirtualKeyboardOptions)
		.VirtualKeyboardTrigger(VirtualKeyboardTrigger)
		.VirtualKeyboardDismissAction(VirtualKeyboardDismissAction)
		.Justification(
#if UE_VERSION_NEWER_THAN(5, 1, 0)
		GetJustification()
#else
		Justification
#endif
		)
		.OverflowPolicy(
#if UE_VERSION_NEWER_THAN(5, 1, 0)
		GetTextOverflowPolicy()
#else
		OverflowPolicy
#endif
		);
	
	// clang-format off
	return SAssignNew(MyVerticalBox, SModioCommonFocusableVerticalBox)
	       + SVerticalBox::Slot()
	       .AutoHeight()
	       [
		       SNew(SBorder)
		       .BorderImage_UObject(this, &UModioCommonEditableTextBox::GetBorderImage)
		       [
			       SNew(SGridPanel).FillColumn(1, 1)
			       + SGridPanel::Slot(0, 0).Padding(StyleCDO->HintIconPadding)
			       [
				       SNew(SScaleBox).Stretch(EStretch::ScaleToFit).StretchDirection(EStretchDirection::Both)
				       [
					       SNew(SImage)
					       .Image_UObject(this, &UModioCommonEditableTextBox::GetHintBrush)
					       .Visibility_UObject(this, &UModioCommonEditableTextBox::GetHintBrushVisibility)
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

void UModioCommonEditableTextBox::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	MyVerticalBox.Reset();
}

const FSlateBrush* UModioCommonEditableTextBox::GetHintBrush() const
{
	UModioCommonEditableTextBoxStyle* StyleCDO = ModioStyle.GetDefaultObject();
	if (!StyleCDO)
	{
		return nullptr;
	}
	return &StyleCDO->HintIcon;
}

EVisibility UModioCommonEditableTextBox::GetHintBrushVisibility() const
{
	if (const UModioCommonEditableTextBoxStyle* StyleCDO = ModioStyle.GetDefaultObject())
	{
		return StyleCDO->bShowHintIcon ? EVisibility::Visible : EVisibility::Collapsed;
	}
	return EVisibility::Collapsed;
}

const FSlateBrush* UModioCommonEditableTextBox::GetBorderImage() const
{
	if (const UModioCommonEditableTextBoxStyle* StyleCDO = ModioStyle.GetDefaultObject())
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

void UModioCommonEditableTextBox::HandleOnTextChanged(const FText& InText)
{
	if (const UModioCommonEditableTextBoxStyle* StyleCDO = ModioStyle.GetDefaultObject())
	{
		if (StyleCDO->MaxTextLength > 0 && InText.ToString().Len() > StyleCDO->MaxTextLength)
		{
			const FString TruncatedString = InText.ToString().Left(StyleCDO->MaxTextLength);
			SetText(FText::FromString(TruncatedString));
			return;
		}
	}
	Super::HandleOnTextChanged(InText);
	OnEditableTextChanged.Broadcast(InText);
}
