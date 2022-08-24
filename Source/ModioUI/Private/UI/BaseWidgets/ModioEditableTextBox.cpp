// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/BaseWidgets/ModioEditableTextBox.h"
#include "Libraries/ModioSDKLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UI/Styles/ModioEditableTextBoxStyle.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SGridPanel.h"
#include "Widgets/Layout/SScaleBox.h"

UModioEditableTextBox::UModioEditableTextBox(const FObjectInitializer& Initializer) : UEditableTextBox(Initializer)
{
	MinimumDesiredWidth = 200;
}

void UModioEditableTextBox::SynchronizeProperties()
{
	UEditableTextBox::SynchronizeProperties();
	const FModioEditableTextBoxStyle* ResolvedStyle = TextBoxStyle.FindStyle<FModioEditableTextBoxStyle>();
	if (ResolvedStyle)
	{
		MyEditableTextBlock->SetStyle(ResolvedStyle);
	}
}
FString UModioEditableTextBox::NativeGatherInput()
{
	return MyEditableTextBlock.IsValid() ? MyEditableTextBlock->GetText().ToString() : FString {};
}

TSharedRef<SWidget> UModioEditableTextBox::RebuildWidget()
{
	UEditableTextBox::RebuildWidget();

	const FModioEditableTextBoxStyle* ResolvedStyle = TextBoxStyle.FindStyle<FModioEditableTextBoxStyle>();
	if (ResolvedStyle)
	{
		MyEditableTextBlock->SetStyle(ResolvedStyle);
	}

	// clang-format off
	return SAssignNew(MyVerticalBox, SVerticalBox)
	+SVerticalBox::Slot()
	.AutoHeight()
	[
		SNew(SBorder)
		.BorderImage_UObject(this, &UModioEditableTextBox::GetBorderImage)
		[
			SNew(SGridPanel).FillColumn(1, 1) 
			+SGridPanel::Slot(0, 0)
			[
				SNew(SScaleBox).Stretch(EStretch::ScaleToFit).StretchDirection(EStretchDirection::Both)
				[
					SNew(SImage)
					.Image_UObject(this, &UModioEditableTextBox::GetHintGlyph)
					.Visibility_UObject(this, &UModioEditableTextBox::GetHintGlyphVisibility)
				]
			] 
			+SGridPanel::Slot(1, 0)
			[
				MyEditableTextBlock.ToSharedRef()
			]
		]
	];
	// clang-format on
}

void UModioEditableTextBox::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	MyErrorTextBlock.Reset();
	MyVerticalBox.Reset();
}

void UModioEditableTextBox::NativeSetHintText(FText InHintText)
{
	if (MyEditableTextBlock)
	{
		MyEditableTextBlock->SetHintText(InHintText);
	}
}

const FSlateBrush* UModioEditableTextBox::GetHintGlyph() const
{
	const FModioEditableTextBoxStyle* ResolvedStyle = TextBoxStyle.FindStyle<FModioEditableTextBoxStyle>();
	if (ResolvedStyle)
	{
		return &ResolvedStyle->HintIcon;
	}
	return nullptr;
}

bool UModioEditableTextBox::HasValidationError() const
{
	return MyErrorTextBlock.IsValid() && !MyErrorTextBlock->GetText().IsEmpty();
}

const FSlateBrush* UModioEditableTextBox::GetBorderImage() const
{
	const FModioEditableTextBoxStyle* ResolvedStyle = TextBoxStyle.FindStyle<FModioEditableTextBoxStyle>();
	if (ResolvedStyle)
	{
		if (HasValidationError())
		{
			return &ResolvedStyle->ErrorBorderBrush;
		}
		if (IsHovered() || HasAnyUserFocus() || HasFocusedDescendants())
		{
			return &ResolvedStyle->HoveredBorderBrush;
		}
		return &ResolvedStyle->NormalBorderBrush;
	}
	return nullptr;
}

EVisibility UModioEditableTextBox::GetHintGlyphVisibility() const
{
	const FModioEditableTextBoxStyle* ResolvedStyle = TextBoxStyle.FindStyle<FModioEditableTextBoxStyle>();
	if (ResolvedStyle)
	{
		return ResolvedStyle->bShowHintIcon ? EVisibility::Visible : EVisibility::Collapsed;
	}
	return EVisibility::Collapsed;
}

void UModioEditableTextBox::NativeSetValidationError(FText ErrorText)
{
	const FModioRichTextStyle* ResolvedErrorTextStyle = TextStyle.FindStyle<FModioRichTextStyle>();
	const FTextBlockStyle* ErrorTextBlockStyle = &FTextBlockStyle::GetDefault();

	if (ResolvedErrorTextStyle)
	{
		TSharedPtr<FSlateStyleSet> StyleSet = ResolvedErrorTextStyle->GetStyleSet();
		if (StyleSet)
		{
			ErrorTextBlockStyle = &StyleSet->GetWidgetStyle<FTextBlockStyle>(FName("Error"));
		}
	}

	if (ErrorText.IsEmpty())
	{
		if (MyErrorTextBlock.IsValid())
		{
			MyErrorTextBlock->SetText(ErrorText);
		}
		return;
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
									 .StyleReference(&TextStyle)
									 .TextStyle(ErrorTextBlockStyle)];
	}
	else
	{
		MyErrorTextBlock->SetText(ErrorText);
	}
}

void UModioEditableTextBox::HandleOnTextChanged(const FText& InText)
{
	Super::HandleOnTextChanged(InText);

	if (bValidateInput)
	{
		FText ValidationError;
		NativeValidateText(MyEditableTextBlock->GetText(), ValidationError);
	}
}