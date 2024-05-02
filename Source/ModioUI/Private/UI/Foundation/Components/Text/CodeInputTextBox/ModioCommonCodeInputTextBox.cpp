/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Foundation/Components/Text/CodeInputTextBox/ModioCommonCodeInputTextBox.h"

#include "ModioUI.h"
#include "UI/Foundation/Components/Extra/SModioCommonFocusableVerticalBox.h"
#include "UI/Foundation/Components/Text/CodeInputTextBox/ModioCommonCodeInputTextBoxStyle.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SEditableText.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"

TSharedRef<SWidget> SModioCommonCodeInputTextBox::CreateCharacterWidget(int32 WidgetIndex)
{
	TSharedRef<STextBlock> Widget = SNew(STextBlock)
										.Justification(ETextJustify::Center)
										.TextStyle(&Style)
										.MinDesiredWidth(Style.MinimumCharacterWidth)
										.Margin(Style.CharacterPadding)
										.Text(TAttribute<FText>::Create(TAttribute<FText>::FGetter::CreateSP(
											this, &SModioCommonCodeInputTextBox::GetCharacterAtIndex, WidgetIndex)));
	CharacterWidgets.Add(Widget);
	return Widget;
}

FReply SModioCommonCodeInputTextBox::OnKeyDownHandler(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::Enter || InKeyEvent.GetKey() == EKeys::Virtual_Accept)
	{
		OnCodeSubmit.ExecuteIfBound();
	}

	return FReply::Unhandled();
}

void SModioCommonCodeInputTextBox::RebuildChildren(uint32 NumChildren)
{
	CharacterWidgets.Empty();
	MyCharacterGrid->ClearChildren();

	// Display widgets need to be looking at the reverse index when we are in a culture which is RTL
	const bool bIsRightToLeft = TextFlowDirection.IsSet() && TextFlowDirection == ETextFlowDirection::RightToLeft;

	for (uint32 ChildIndex = 0; ChildIndex < NumChildren; ChildIndex++)
	{
		const int32 ColumnIndex = bIsRightToLeft ? NumChildren - ChildIndex - 1 : ChildIndex;
		SUniformGridPanel::FSlot& CurrentSlot = *MyCharacterGrid->AddSlot(ColumnIndex, 0).GetSlot();
		CurrentSlot.SetHorizontalAlignment(HAlign_Center);
		CurrentSlot.SetVerticalAlignment(VAlign_Center);
		CurrentSlot.AttachWidget(
			SNew(SBorder).BorderImage(this, &SModioCommonCodeInputTextBox::GetCharacterBorder)
				[SNew(SOverlay) +
				 SOverlay::Slot()[SNew(SImage)
									  .Image(&Style.FakeCaretBrush)
									  .Visibility(this, &SModioCommonCodeInputTextBox::GetCaretVisibility, static_cast<uint32>(ChildIndex))]
									  .HAlign(Style.FakeCaretHorizontalAlignment)
									  .VAlign(Style.FakeCaretVerticalAlignment) +
				 SOverlay::Slot()[CreateCharacterWidget(ChildIndex)]]);
	}

	Invalidate(EInvalidateWidgetReason::Layout);
}

FReply SModioCommonCodeInputTextBox::OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent)
{
	FSlateApplication::Get().SetAllUserFocus(HiddenInputField);
	return FReply::Handled();
}

bool SModioCommonCodeInputTextBox::SupportsKeyboardFocus() const
{
	return true;
}

void SModioCommonCodeInputTextBox::OnInputTextChanged(const FText& NewText)
{
	if (!HiddenInputField.IsValid())
	{
		UE_LOG(ModioUI, Error, TEXT("Unable to process input text change in SModioCommonCodeInputTextBox: HiddenInputField is not valid"));
		return;
	}

	const FString TruncatedText = NewText.ToString().Mid(0, CharacterWidgets.Num()).ToUpper();
	if (HiddenInputField->GetText().ToString() != TruncatedText)
	{
		HiddenInputField->SetText(FText::FromString(TruncatedText));
	}
	else if (NewText.ToString().Len() > CharacterWidgets.Num())
	{
		HiddenInputField->SetText(FText::FromString(TruncatedText));
	}
}

FText SModioCommonCodeInputTextBox::GetCharacterAtIndex(int32 Index) const
{
	if (!HiddenInputField.IsValid())
	{
		UE_LOG(ModioUI, Error, TEXT("Unable to get character at index %d in SModioCommonCodeInputTextBox: HiddenInputField is not valid"), Index);
		return FText::GetEmpty();
	}

	const FString CurrentText = HiddenInputField->GetText().ToString();
	if (Index < CurrentText.Len())
	{
		return FText::FromString(CurrentText.Mid(Index, 1));
	}
	return FText::GetEmpty();
}

const FSlateBrush* SModioCommonCodeInputTextBox::GetCharacterBorder() const
{
	if (FSlateApplication::Get().HasFocusedDescendants(AsShared()))
	{
		CachedCharacterBrush = Style.CharacterBorderBrushFocused;
	}
	else
	{
		CachedCharacterBrush = Style.CharacterBorderBrushBase;
	}
	CachedCharacterBrush.Margin = Style.CharacterBorderPadding;
	return &CachedCharacterBrush;
}

EVisibility SModioCommonCodeInputTextBox::GetCaretVisibility(uint32 Index) const
{
	if (!HiddenInputField.IsValid())
	{
		UE_LOG(ModioUI, Error, TEXT("Unable to get caret visibility at index %d in SModioCommonCodeInputTextBox: HiddenInputField is not valid"), Index);
		return EVisibility::Hidden;
	}

	if (Index == HiddenInputField->GetText().ToString().Len())
	{
		if (FSlateApplication::Get().HasFocusedDescendants(AsShared()))
		{
			return EVisibility::Visible;
		}
	}
	return EVisibility::Hidden;
}

int32 SModioCommonCodeInputTextBox::GetNumOfCharacters() const
{
	return CharacterWidgets.Num();
}

void SModioCommonCodeInputTextBox::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;
	// clang-format off
	ChildSlot
	[
		SNew(SVerticalBox)
		+SVerticalBox::Slot()
		.AutoHeight()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Fill)
		[
			SAssignNew(MyCharacterGrid, SUniformGridPanel)
		]
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Top)
		.Padding(FMargin(0,0,0,24))
		[
			SNew(SBox)
			.WidthOverride(1)
			.HeightOverride(1)
			[
				SAssignNew(HiddenInputField, SEditableText)
				.CaretImage(TAttribute<const FSlateBrush*>())
				.OnKeyDownHandler(this, &SModioCommonCodeInputTextBox::OnKeyDownHandler)
				.OnTextChanged(this, &SModioCommonCodeInputTextBox::OnInputTextChanged)
				.VirtualKeyboardOptions(InArgs._VirtualKeyboardOptions)
				.VirtualKeyboardTrigger(InArgs._VirtualKeyboardTrigger)
				.VirtualKeyboardDismissAction(InArgs._VirtualKeyboardDismissAction)
			]
		]
	];
	// clang-format on

	Style = InArgs._Style;
	MyCharacterGrid->SetSlotPadding(Style.CharacterSpacing);
	OnCodeSubmit = InArgs._OnCodeSubmit;
	TextFlowDirection = InArgs._TextFlowDirection;
	HiddenInputField->SetHintText(Style.HintText);

	RebuildChildren(InArgs._NumChildren);
}

void SModioCommonCodeInputTextBox::SetStyle(const FModioCommonCodeInputTextBoxInputStyle& NewStyle)
{
	Style = NewStyle;
	if (MyCharacterGrid)
	{
		MyCharacterGrid->SetSlotPadding(Style.CharacterSpacing);
		for (const TSharedPtr<STextBlock>& Child : CharacterWidgets)
		{
			if (Child)
			{
				Child->SetTextStyle(&Style);
			}
		}
	}
	Invalidate(EInvalidateWidgetReason::Layout | EInvalidateWidgetReason::Paint);
}

void SModioCommonCodeInputTextBox::SetNumChildren(int32 NewNumChildren)
{
	if (NewNumChildren != GetNumOfCharacters())
	{
		RebuildChildren(NewNumChildren);
	}
}

FString SModioCommonCodeInputTextBox::GetCodeString() const
{
	if (!HiddenInputField.IsValid())
	{
		UE_LOG(ModioUI, Error, TEXT("Unable to get code string in SModioCommonCodeInputTextBox: HiddenInputField is not valid"));
		return FString();
	}
	const FString CurrentText = HiddenInputField->GetText().ToString();
	UE_LOG(ModioUI, Warning, TEXT("Returning CurrentText '%s' from SModioCommonCodeInputTextBox::GetCodeString"), *CurrentText);
	return CurrentText;
}

void SModioCommonCodeInputTextBox::SetCodeString(const FString& NewCodeString)
{
	OnInputTextChanged(FText::FromString(NewCodeString));
}

void UModioCommonCodeInputTextBox::SetStyle(TSubclassOf<UModioCommonCodeInputTextBoxStyle> InStyle)
{
	if (InStyle && InStyle != ModioStyle)
	{
		ModioStyle = InStyle;
		SynchronizeProperties();
	}
}

FString UModioCommonCodeInputTextBox::GetCodeString() const
{
	if (MyInput)
	{
		return MyInput->GetCodeString();
	}
	UE_LOG(ModioUI, Error, TEXT("Unable to get code string in UModioCommonCodeInputTextBox: MyInput is not valid"));
	return FString();
}

void UModioCommonCodeInputTextBox::SetCodeString(FString InCodeString)
{
	if (MyInput)
	{
		MyInput->SetCodeString(InCodeString);
	}
	else
	{
		UE_LOG(ModioUI, Error, TEXT("Unable to set code string in UModioCommonCodeInputTextBox: MyInput is not valid"));
	}
}

TSharedRef<SWidget> UModioCommonCodeInputTextBox::RebuildWidget()
{
	return SAssignNew(MyVerticalBox, SModioCommonFocusableVerticalBox) +
		   SVerticalBox::Slot()
			   .AutoHeight()[SAssignNew(MyInput, SModioCommonCodeInputTextBox)
								 .Style(ModioStyle ? ModioStyle.GetDefaultObject()->Style : FModioCommonCodeInputTextBoxInputStyle())
								 .NumChildren(NumberOfCharacters)
								 .OnCodeSubmit(SModioCommonCodeInputTextBox::FOnCodeSubmit::CreateWeakLambda(this, [this]() {
									 OnCodeSubmit.Broadcast();
									 OnCodeSubmitDynamic.Broadcast();
								 }))
								 .TextFlowDirection(TextFlowDirection)
								 .VirtualKeyboardOptions(VirtualKeyboardOptions)
								 .VirtualKeyboardTrigger(VirtualKeyboardTrigger)
								 .VirtualKeyboardDismissAction(VirtualKeyboardDismissAction)];
}

void UModioCommonCodeInputTextBox::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	MyVerticalBox.Reset();
}
