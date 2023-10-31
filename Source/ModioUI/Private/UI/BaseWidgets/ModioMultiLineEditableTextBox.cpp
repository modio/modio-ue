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
}

FReply SModioMultiLineEditableTextBox::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	const FKey Key = InKeyEvent.GetKey();

	bool bIsHandled = false;
	FKey KeyToHandle = EKeys::Invalid;

	if (Key == EKeys::Gamepad_RightStick_Up || Key == EKeys::Gamepad_LeftStick_Up || Key == EKeys::Gamepad_DPad_Up)
	{
		FString TextLine;
		EditableText->GetCurrentTextLine(TextLine);
		if (LastCursorPosition.GetLineIndex() == 0)
		{
			bIsHandled = false;
		}
		else
		{
			bIsHandled = true;
			KeyToHandle = EKeys::Up;
		}
	}
	else if (Key == EKeys::Gamepad_RightStick_Down || Key == EKeys::Gamepad_LeftStick_Down || Key == EKeys::Gamepad_DPad_Down)
	{
		FString LastLine = [this]() -> FString {
			TArray<FString> Lines;
			EditableText->GetText().ToString().ParseIntoArrayLines(Lines);
			return Lines.Num() > 0 ? Lines.Last() : FString();
		}();

		FString TextLine;
		EditableText->GetCurrentTextLine(TextLine);
		if (TextLine.Find(LastLine) != INDEX_NONE)
		{
			bIsHandled = false;
		}
		else
		{
			bIsHandled = true;
			KeyToHandle = EKeys::Down;
		}
	}
	else if (Key == EKeys::Gamepad_RightStick_Left || Key == EKeys::Gamepad_LeftStick_Left || Key == EKeys::Gamepad_DPad_Left)
	{
		bIsHandled = true;
		KeyToHandle = EKeys::Left;
	}
	else if (Key == EKeys::Gamepad_RightStick_Right || Key == EKeys::Gamepad_LeftStick_Right || Key == EKeys::Gamepad_DPad_Right)
	{
		bIsHandled = true;
		KeyToHandle = EKeys::Right;
	}

	if (bIsHandled)
	{
		return StaticCastSharedPtr<SWidget>(EditableText)->OnKeyDown(MyGeometry, FKeyEvent(KeyToHandle, FModifierKeysState(), 0, false, 0, 0));
	}
	return FReply::Unhandled();
}

void SModioMultiLineEditableTextBox::HandleOnTextCursorMoved(const FTextLocation& NewCursorPosition)
{
	LastCursorPosition = NewCursorPosition;
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
