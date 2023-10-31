/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/CommonComponents/ModioRichTextButton.h"
#include "Components/ButtonSlot.h"
#include "Engine/Engine.h"
#include "Misc/EngineVersionComparison.h"
#include "ModioUISubsystem.h"
#include "Settings/ModioUISettings.h"
#include "UI/BaseWidgets/ModioRichTextBlockDecorator.h"
#include "UI/Styles/ModioButtonStyle.h"
#include "UI/Styles/ModioRichTextStyle.h"
#include "Widgets/Layout/SScaleBox.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Input/SButton.h"

void UModioRichTextButton::SynchronizeProperties()
{
	ButtonContent->SetJustification(Justification);
	ButtonContent->SetDefaultStyleName(DefaultStyleName);
	if (UButtonSlot* ButtonSlot = Cast<UButtonSlot>(GetContentSlot()))
	{
		ButtonSlot->SetVerticalAlignment(VAlign_Center);
	}
	Super::SynchronizeProperties();
}

TSharedRef<SWidget> UModioRichTextButton::RebuildWidget()
{
	TSharedRef<SWidget> Widget = Super::RebuildWidget();

	if (!ButtonContent)
	{
		ButtonContent = NewObject<UModioRichTextBlock>(this);
	}
	if (!InputHintImage)
	{
		InputHintImage = NewObject<UModioInputBindingImage>(this);
#if UE_VERSION_NEWER_THAN(5, 0, 0)
		InputHintImage->SetDesiredSizeOverride(FVector2D(32));
#else
		InputHintImage->SetBrushSize(FVector2D(24));
#endif
		InputHintImage->SetVisibility(ESlateVisibility::Collapsed);
		InputHintImage->GetInputModeVisibilityDelegate().BindDynamic(this,
																	 &UModioRichTextButton::GetInputHintVisibility);

		if (KeyForInputHint.IsValid())
		{
			InputHintImage->SetKeyToShow(KeyForInputHint);
		}
	}
	UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();

	ButtonContent->SetText(Subsystem ? Subsystem->FormatText(ButtonLabel) : ButtonLabel);
	ButtonContent->AddDecorator(UModioRichTextBlockDecorator::StaticClass());
	ButtonContent->GetStyleDelegate().BindUObject(this, &UModioRichTextButton::GetRichTextStyle);
	// clang-format off
	SAssignNew(MyContentGrid, SGridPanel)
	+ SGridPanel::Slot(0, 0)
	.VAlign(VAlign_Center)
	[
		SNew(SBox)
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Center)
		[
			SNew(SScaleBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			.Stretch(EStretch::ScaleToFitX)
			.StretchDirection(EStretchDirection::DownOnly)
			[
				ButtonContent->TakeWidget()
			]
		]
	]
	+ SGridPanel::Slot(1,0)
	.HAlign(HAlign_Right)
	.VAlign(VAlign_Center)
	.Padding(FMargin(8, 0, 0, 0))
	.Expose(MyHintImageSlot)
	[
		SNew(SBox)
		.MaxDesiredHeight(20)
		.MaxDesiredWidth(20)
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Center)
		[
			SNew(SScaleBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			.Stretch(EStretch::ScaleToFit)
			.StretchDirection(EStretchDirection::DownOnly)
			[
				InputHintImage->TakeWidget()
			]
		]
	];
	// clang-format on
	MyContentGrid->SetColumnFill(0, 1);
	 
	MyButton->SetContent(MyContentGrid.ToSharedRef());
	MyButton->SetHAlign(HAlign_Fill);
	MyButton->SetVAlign(VAlign_Center);
	return Widget;
}

TArray<FString> UModioRichTextButton::GetStyleNames() const
{
	TArray<FString> StyleNames;
	const FModioRichTextStyle& RichTextStyle = GetRichTextStyle();
	TArray<FName> TmpStyleNames;
	RichTextStyle.Styles.GenerateKeyArray(TmpStyleNames);
	for (const FName& Name : TmpStyleNames)
	{
		StyleNames.Add(Name.ToString());
	}

	if (StyleNames.Num() == 0)
	{
		StyleNames.Add("Default");
	}
	return StyleNames;
}

void UModioRichTextButton::NativeDisplayHintForInput(FKey VirtualKey)
{
	IModioUIInputHintDisplayWidget::NativeDisplayHintForInput(VirtualKey);
	KeyForInputHint = VirtualKey;
	UModioUI4Subsystem* subsystem = GEngine->GetEngineSubsystem<UModioUI4Subsystem>();
	if (InputHintImage && subsystem && KeyForInputHint.IsValid())
	{
		InputHintImage->LastDeviceType = subsystem->GetLastInputDevice();
		InputHintImage->SetKeyToShow(KeyForInputHint);
	}
}

ESlateVisibility UModioRichTextButton::GetInputHintVisibility(EModioUIInputMode InputMode)
{
	UModioUISettings* Settings = UModioUISettings::StaticClass()->GetDefaultObject<UModioUISettings>();

	if (Settings->bDisableInputGlyphsCompletely || !bIsInputHintVisible)
	{
		return ESlateVisibility::Collapsed;
	}

	if (InputMode == EModioUIInputMode::Unknown)
	{
		return ESlateVisibility::Collapsed;
	}
	else
	{
		return ESlateVisibility::SelfHitTestInvisible;
	}
}

void UModioRichTextButton::SetInputHintVisibility(bool bVisible) 
{
	bIsInputHintVisible = bVisible;
	InputHintImage->SetVisibility(bIsInputHintVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

const FModioRichTextStyle& UModioRichTextButton::GetRichTextStyle() const
{
	const FModioButtonStyle* Style = ButtonStyle.FindStyle<FModioButtonStyle>();
	if (Style)
	{
		const FModioRichTextStyle* RichTextStyle = Style->RichTextStyle.FindStyle<FModioRichTextStyle>();
		if (RichTextStyle)
		{
			return *RichTextStyle;
		}
	}

	FModioUIStyleRef DefaultStyle = {"DefaultButtonRichTextStyle"};
	const FModioRichTextStyle* RichTextStyle = DefaultStyle.FindStyle<FModioRichTextStyle>();
	return RichTextStyle ? *RichTextStyle : FModioRichTextStyle::GetDefault();
}

void UModioRichTextButton::SetLabel(FText NewLabel)
{
	ButtonLabel = NewLabel;
	UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
	if (!ButtonContent)
	{
		RebuildWidget();
	}
	if (ButtonContent)
	{
		ButtonContent->SetText(Subsystem ? Subsystem->FormatText(ButtonLabel) : ButtonLabel);
	}
}

FString UModioRichTextButton::GetLabel()
{
	return ButtonLabel.ToString();
}

void UModioRichTextButton::SetJustification(ETextJustify::Type NewJustification)
{
	Justification = NewJustification;
}
