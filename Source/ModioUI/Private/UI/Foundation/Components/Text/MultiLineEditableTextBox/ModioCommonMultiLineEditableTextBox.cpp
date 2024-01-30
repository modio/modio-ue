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

#include "ModioUI.h"
#include "UI/Foundation/Components/Text/MultiLineEditableTextBox/ModioCommonMultiLineEditableTextBoxStyle.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"
#include "Widgets/Layout/SGridPanel.h"
#include "Widgets/Layout/SScaleBox.h"
#include "Styling/SlateTypes.h"
#include "UI/Foundation/Components/Extra/SModioCommonFocusableVerticalBox.h"

FReply SModioCommonMultiLineEditableTextBox::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	if (bIgnoreOnKeyDown)
	{
		return FReply::Unhandled();
	}

	const FKey Key = InKeyEvent.GetKey();

	if (Key == EKeys::Up || Key == EKeys::Gamepad_RightStick_Up || Key == EKeys::Gamepad_DPad_Up)
	{
		return HandleNavigation(MyGeometry, EUINavigation::Up);
	}
	if (Key == EKeys::Down || Key == EKeys::Gamepad_RightStick_Down || Key == EKeys::Gamepad_DPad_Down)
	{
		return HandleNavigation(MyGeometry, EUINavigation::Down);
	}
	if (Key == EKeys::Left || Key == EKeys::Gamepad_RightStick_Left || Key == EKeys::Gamepad_LeftStick_Left || Key == EKeys::Gamepad_DPad_Left)
	{
		return HandleNavigation(MyGeometry, EUINavigation::Left);
	}
	if (Key == EKeys::Right || Key == EKeys::Gamepad_RightStick_Right || Key == EKeys::Gamepad_LeftStick_Right || Key == EKeys::Gamepad_DPad_Right)
	{
		return HandleNavigation(MyGeometry, EUINavigation::Right);
	}

	return FReply::Unhandled();
}

FReply SModioCommonMultiLineEditableTextBox::HandleNavigation(const FGeometry& MyGeometry, EUINavigation Navigation)
{
	TSharedPtr<SWidget> EditableTextWidget = StaticCastSharedPtr<SWidget>(EditableText);
	if (!EditableTextWidget)
	{
		UE_LOG(ModioUI, Error, TEXT("Unable to handle '%s' navigation for the multi line text box"), *UEnum::GetValueAsString(Navigation));
		return FReply::Unhandled();
	}

	UE_LOG(ModioUI, Log, TEXT("Sending '%s' navigation to the multiline editable text"), *UEnum::GetValueAsString(Navigation));

	if (Navigation == EUINavigation::Left || Navigation == EUINavigation::Right)
	{
		FKey LeftOrRightKey = Navigation == EUINavigation::Left ? EKeys::Left : EKeys::Right;
		FKeyEvent LeftOrRightEvent(LeftOrRightKey, FSlateApplication::Get().GetModifierKeys(), 0, false, 0, 0);
		bIgnoreOnKeyDown = true;
		FReply HandledEvent = EditableTextWidget->OnKeyDown(MyGeometry, LeftOrRightEvent);
		bIgnoreOnKeyDown = false;
		return HandledEvent;
	}

	if (Navigation == EUINavigation::Up || Navigation == EUINavigation::Down)
	{
		FKey UpOrDownKey = Navigation == EUINavigation::Up ? EKeys::Up : EKeys::Down;
		const FTextLocation PriorCursorPosition = EditableText->GetCursorLocation();
		FKeyEvent UpOrDownEvent(UpOrDownKey, FSlateApplication::Get().GetModifierKeys(), 0, false, 0, 0);
		bIgnoreOnKeyDown = true;
		FReply HandledEvent = EditableTextWidget->OnKeyDown(MyGeometry, UpOrDownEvent);
		bIgnoreOnKeyDown = false;
		const FTextLocation NewCursorPosition = EditableText->GetCursorLocation();

		if (HandledEvent.IsEventHandled())
		{
			// If the cursor changed position, the editable text handled the event by moving the cursor
			if (PriorCursorPosition != NewCursorPosition)
			{
				return HandledEvent;
			}

			// Otherwise, simulate up/down navigation using the "Gamepad_LeftY" key to go beyond this widget
			const float AnalogValue = Navigation == EUINavigation::Up ? 1.0f : -1.0f;
			FAnalogInputEvent GamepadYEvent(EKeys::Gamepad_LeftY, FSlateApplication::Get().GetModifierKeys(), 0, false, 0, 0, AnalogValue);
			FSlateApplication::Get().ProcessAnalogInputEvent(GamepadYEvent);
			return FReply::Handled();
		}
	}

	return FReply::Unhandled();
}

void SModioCommonMultiLineEditableTextBox::OnFocusChanging(const FWeakWidgetPath& PreviousFocusPath, const FWidgetPath& NewWidgetPath, const FFocusEvent& InFocusEvent)
{
	SMultiLineEditableTextBox::OnFocusChanging(PreviousFocusPath, NewWidgetPath, InFocusEvent);

	// This is needed to apply the focused style to the text box even when the text is read-only
	if (bApplyFocusedStyleInReadOnlyMode && EditableText && Style && EditableText->IsTextReadOnly())
	{
		if (!bIsTextBoxFocused && NewWidgetPath.IsValid() && NewWidgetPath.ContainsWidget(this))
		{
			bIsTextBoxFocused = true;
			EditableTextBoxStyle = *Style;
			const_cast<FEditableTextBoxStyle*>(Style)->BackgroundImageReadOnly = Style->BackgroundImageFocused;
			SetStyle(Style);
		}
		else
		{
			const_cast<FEditableTextBoxStyle*>(Style)->BackgroundImageReadOnly = EditableTextBoxStyle.BackgroundImageReadOnly;
			SetStyle(Style);
			bIsTextBoxFocused = false;
		}
	}
}

void UModioCommonMultiLineEditableTextBox::SetStyle(TSubclassOf<UModioCommonMultiLineEditableTextBoxStyle> InStyle)
{
	if (InStyle && InStyle != ModioStyle)
	{
		ModioStyle = InStyle;
		SynchronizeProperties();
	}
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
		if (SModioCommonMultiLineEditableTextBox* EditableTextWidget = StaticCastSharedPtr<SModioCommonMultiLineEditableTextBox>(MyEditableTextBlock).Get())
		{
			EditableTextWidget->SetApplyFocusedStyleInReadOnlyMode(StyleCDO->bApplyFocusedStyleInReadOnlyMode);
		}
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
		.OnTextChanged(BIND_UOBJECT_DELEGATE(FOnTextChanged, HandleOnCommonTextChanged))
		.OnTextCommitted(BIND_UOBJECT_DELEGATE(FOnTextCommitted, HandleOnTextCommitted))
		//.VirtualKeyboardType(EVirtualKeyboardType::AsKeyboardType(KeyboardType.GetValue()))
		.VirtualKeyboardOptions(VirtualKeyboardOptions)
		.VirtualKeyboardTrigger(VirtualKeyboardTrigger)
		.VirtualKeyboardDismissAction(VirtualKeyboardDismissAction)
		.Justification(Justification)
		.OverflowPolicy(OverflowPolicy)
		.AlwaysShowScrollbars(AlwaysShowScrollbars)
		.AllowMultiLine(AllowMultiLine);

	MyEditableTextBlock->SetOnKeyDownHandler(FOnKeyDown::CreateWeakLambda(this, [this](const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) -> FReply
	{
		if (TSharedPtr<SWidget> WidgetToHandle = StaticCastSharedPtr<SWidget>(MyEditableTextBlock))
		{
			return WidgetToHandle->OnKeyDown(MyGeometry, InKeyEvent);
		}
		return FReply::Unhandled();
	}));
	
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

void UModioCommonMultiLineEditableTextBox::HandleOnCommonTextChanged(const FText& InText)
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
	HandleOnTextChanged(InText);
	OnMultiLineEditableTextChanged.Broadcast(InText);
}
