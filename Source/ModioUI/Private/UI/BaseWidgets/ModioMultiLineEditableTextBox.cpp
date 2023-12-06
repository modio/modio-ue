/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/BaseWidgets/ModioMultiLineEditableTextBox.h"
#include "UI/Styles/ModioEditableTextBoxStyle.h"
#include "UI/Styles/ModioMultiLineEditableTextBoxStyle.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SGridPanel.h"
#include "Widgets/Layout/SScaleBox.h"

FString UModioMultiLineEditableTextBox::NativeGatherInput()
{
	return GetText().ToString();
}

void UModioMultiLineEditableTextBox::NativeSetHintText(FText InHintText)
{
	if (MyEditableTextBlock)
	{
		MyEditableTextBlock->SetHintText(InHintText);
	}
}

void UModioMultiLineEditableTextBox::NativeSetInput(const FString& Input)
{
	if (MyEditableTextBlock)
	{
		MyEditableTextBlock->SetText(FText::FromString(Input));
	}
}

TSharedRef<SWidget> UModioMultiLineEditableTextBox::RebuildWidget()
{
	MyEditableTextBlock = SNew(SModioMultiLineEditableTextBox)
							  .Style(&WidgetStyle)
#if !UE_VERSION_OLDER_THAN(5, 1, 0)
							  .TextStyle(&WidgetStyle.TextStyle)
#else
							  .TextStyle(&TextStyle)
#endif
							  .AllowContextMenu(AllowContextMenu)
							  .WrappingPolicy(ETextWrappingPolicy::AllowPerCharacterWrapping)
							  .WrapTextAt(0.0f)
							  .AutoWrapText(true)
							  .AllowMultiLine(true)
							  .IsReadOnly(bIsReadOnly)
							  .VirtualKeyboardOptions(VirtualKeyboardOptions)
							  .VirtualKeyboardDismissAction(VirtualKeyboardDismissAction)
							  .OnTextChanged(BIND_UOBJECT_DELEGATE(FOnTextChanged, HandleOnTextChangedDelegate))
							  .OnTextCommitted(BIND_UOBJECT_DELEGATE(FOnTextCommitted, HandleOnTextCommitted))
							  .OnCursorMoved_UObject(this, &UModioMultiLineEditableTextBox::HandleOnTextCursorMoved);

	// clang-format off
	return SAssignNew(MyVerticalBox, SVerticalBox)
	+SVerticalBox::Slot()
	.FillHeight(128)
	[
		MyEditableTextBlock.ToSharedRef()
	];
	// clang-format on
}

bool UModioMultiLineEditableTextBox::HasValidationError() const
{
	return false;
}

void UModioMultiLineEditableTextBox::NativeSetValidationError(FText ErrorText)
{
	if (ErrorText.IsEmpty())
	{
		if (MyErrorTextBlock.IsValid())
		{
			MyErrorTextBlock->SetVisibility(EVisibility::Collapsed);
		}
		return;
	}

	const FModioRichTextStyle* ResolvedErrorTextStyle = ErrorTextStyle.FindStyle<FModioRichTextStyle>();
	const FTextBlockStyle* ErrorTextBlockStyle = &FTextBlockStyle::GetDefault();
	const FTextBlockStyle* CharacterOverLimitStyle = &FTextBlockStyle::GetDefault();
	if (ResolvedErrorTextStyle)
	{
		ErrorStyleSet = ResolvedErrorTextStyle->CloneStyleSet();
		if (ErrorStyleSet)
		{
			ErrorTextBlockStyle = &ErrorStyleSet->GetWidgetStyle<FTextBlockStyle>(FName("Error"));
			CharacterOverLimitStyle = &ErrorStyleSet->GetWidgetStyle<FTextBlockStyle>(FName("Default"));
		}
	}

	if (!MyErrorTextBlock.IsValid())
	{
		MyVerticalBox->AddSlot()
			.AutoHeight()
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Center)
			.Padding(0, 8, 0, 0)[SAssignNew(MyErrorTextBlock, SModioRichTextBlock)
									 .Text(ErrorText)
									 .WrapTextAt(700)
									 .StyleReference(&ErrorTextStyle)
									 .TextStyle(ErrorTextBlockStyle)];
	}
	else
	{
		MyErrorTextBlock->SetVisibility(EVisibility::Visible);
		MyErrorTextBlock->SetText(ErrorText);
	}
}

void UModioMultiLineEditableTextBox::HandleOnTextChangedDelegate(const FText& InText)
{
	// Ensure we call base class method so Blueprint bindings are also dispatched
	FText ValidationError;
	if (bValidateInput)
	{
		if (!NativeValidateText(MyEditableTextBlock->GetText(), ValidationError))
		{
			return;
		}
	}

	HandleOnTextChanged(InText);
}

void UModioMultiLineEditableTextBox::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	const FModioMultiLineEditableTextBoxStyle* ResolvedStyle =
		TextBoxStyle.FindStyle<FModioMultiLineEditableTextBoxStyle>();
	if (ResolvedStyle)
	{
		MyEditableTextBlock->SetStyle(ResolvedStyle);
		const FTextBlockStyle* ResolvedTextStyle = ResolvedStyle->ModioTextStyle.FindStyle<FTextBlockStyle>();
		if (ResolvedTextStyle)
		{
			MyEditableTextBlock->SetTextStyle(ResolvedTextStyle);
		}
	}

	if (SModioMultiLineEditableTextBox* EditableTextWidget = StaticCastSharedPtr<SModioMultiLineEditableTextBox>(MyEditableTextBlock).Get())
	{
		EditableTextWidget->SetApplyFocusedStyleInReadOnlyMode(bApplyFocusedStyleInReadOnlyMode);
	}
}

FReply SModioMultiLineEditableTextBox::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
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

FReply SModioMultiLineEditableTextBox::HandleNavigation(const FGeometry& MyGeometry, EUINavigation Navigation)
{
	TSharedPtr<SWidget> EditableTextWidget = StaticCastSharedPtr<SWidget>(EditableText);
	if (!EditableTextWidget)
	{
		UE_LOG(LogModioUI, Error, TEXT("Unable to handle '%s' navigation for the multi line text box"), *UEnum::GetValueAsString(Navigation));
		return FReply::Unhandled();
	}

	UE_LOG(LogModioUI, Log, TEXT("Sending '%s' navigation to the multiline editable text"), *UEnum::GetValueAsString(Navigation));

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
		FKeyEvent UpOrDownEvent(UpOrDownKey, FSlateApplication::Get().GetModifierKeys(), 0, false, 0, 0);
		bIgnoreOnKeyDown = true;
		FReply HandledEvent = EditableTextWidget->OnKeyDown(MyGeometry, UpOrDownEvent);
		bIgnoreOnKeyDown = false;
		return HandledEvent;
	}

	return FReply::Unhandled();
}

void SModioMultiLineEditableTextBox::HandleOnTextCursorMoved(const FTextLocation& NewCursorPosition)
{
	LastCursorPosition = NewCursorPosition;
}

void SModioMultiLineEditableTextBox::OnFocusChanging(const FWeakWidgetPath& PreviousFocusPath, const FWidgetPath& NewWidgetPath, const FFocusEvent& InFocusEvent)
{
	SMultiLineEditableTextBox::OnFocusChanging(PreviousFocusPath, NewWidgetPath, InFocusEvent);

	// This is needed to apply the focused style to the text box even when the text is read-only
	if (bApplyFocusedStyleInReadOnlyMode && EditableText && Style && EditableText->IsTextReadOnly())
	{
		if (!bIsTextBoxFocused && NewWidgetPath.IsValid() && NewWidgetPath.ContainsWidget(SharedThis(this)))
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

void UModioMultiLineEditableTextBox::HandleOnTextCursorMoved(const FTextLocation& NewCursorPosition)
{
	if (TSharedPtr<SModioMultiLineEditableTextBox> EditableTextBlock = StaticCastSharedPtr<SModioMultiLineEditableTextBox>(MyEditableTextBlock))
	{
		EditableTextBlock->HandleOnTextCursorMoved(NewCursorPosition);
	}
}

TSharedPtr<SMultiLineEditableTextBox> UModioMultiLineEditableTextBox::GetEditableTextBox()
{
	return MyEditableTextBlock;
}
