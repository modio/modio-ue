// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/BaseWidgets/ModioMultiLineEditableTextBox.h"
#include "UI/Styles/ModioMultiLineEditableTextBoxStyle.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"
#include "UI/Styles/ModioEditableTextBoxStyle.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SEditableTextBox.h"
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
	MyEditableTextBlock = SNew(SMultiLineEditableTextBox)
		.Style(&WidgetStyle)
		.TextStyle(&TextStyle)
		.AllowContextMenu(AllowContextMenu)
		.IsReadOnly(bIsReadOnly)
		.VirtualKeyboardOptions(VirtualKeyboardOptions)
		.VirtualKeyboardDismissAction(VirtualKeyboardDismissAction)
		.OnTextChanged(BIND_UOBJECT_DELEGATE(FOnTextChanged, HandleOnTextChangedDelegate))
		.OnTextCommitted(BIND_UOBJECT_DELEGATE(FOnTextCommitted, HandleOnTextCommitted))
		;
	
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
			MyErrorTextBlock->SetText(ErrorText);	
		}
		return;
	}

	const FModioRichTextStyle* ResolvedErrorTextStyle = ErrorTextStyle.FindStyle<FModioRichTextStyle>();
	const FTextBlockStyle *ErrorTextBlockStyle = &FTextBlockStyle::GetDefault();
	
	if (ResolvedErrorTextStyle)
	{
		TSharedPtr<FSlateStyleSet> StyleSet = ResolvedErrorTextStyle->GetStyleSet();
		if (StyleSet)
		{
			ErrorTextBlockStyle = &StyleSet->GetWidgetStyle<FTextBlockStyle>(FName("Error"));
		}
	}
	
	if (!MyErrorTextBlock.IsValid())
	{
		MyVerticalBox->AddSlot()
		.AutoHeight()
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Center)
		.Padding(0, 8, 0, 0)
		[
			SAssignNew(MyErrorTextBlock, SModioRichTextBlock)
			.Text(ErrorText)
			.WrapTextAt(700)
			.StyleReference(&ErrorTextStyle)
			.TextStyle(ErrorTextBlockStyle)
		];
	} else
	{
		MyErrorTextBlock->SetText(ErrorText);
	}
}


void UModioMultiLineEditableTextBox::HandleOnTextChangedDelegate(const FText& InText)
{
	// Ensure we call base class method so Blueprint bindings are also dispatched
	HandleOnTextChanged(InText);

	if (bValidateInput)
	{
		FText ValidationError;
		NativeValidateText(MyEditableTextBlock->GetText(), ValidationError);
	}
}

void UModioMultiLineEditableTextBox::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	const FModioMultiLineEditableTextBoxStyle* ResolvedStyle =
		TextBoxStyle.FindStyle<FModioMultiLineEditableTextBoxStyle>();
	if (ResolvedStyle)
	{
		MyEditableTextBlock->SetStyle(ResolvedStyle);
		const FTextBlockStyle* ResolvedTextStyle = ResolvedStyle->TextStyle.FindStyle<FTextBlockStyle>();
		if (ResolvedTextStyle)
		{
			MyEditableTextBlock->SetTextStyle(ResolvedTextStyle);
		}
	}
}
