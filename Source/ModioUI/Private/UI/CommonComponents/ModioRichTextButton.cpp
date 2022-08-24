// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/CommonComponents/ModioRichTextButton.h"
#include "Components/ButtonSlot.h"
#include "Engine/Engine.h"
#include "Misc/EngineVersionComparison.h"
#include "ModioUISubsystem.h"
#include "UI/BaseWidgets/ModioRichTextBlockDecorator.h"
#include "UI/Styles/ModioButtonStyle.h"
#include "UI/Styles/ModioRichTextStyle.h"
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
		InputHintImage->SetDesiredSizeOverride(FVector2D(20));
#else
		InputHintImage->SetBrushSize(FVector2D(20));
#endif
		InputHintImage->SetVisibility(ESlateVisibility::Collapsed);
		InputHintImage->GetInputModeVisibilityDelegate().BindDynamic(this,
																	 &UModioRichTextButton::GetInputHintVisibility);
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
		ButtonContent->TakeWidget()
	]
	+ SGridPanel::Slot(1,0)
	.Expose(MyHintImageSlot)
	[
		InputHintImage->TakeWidget()
	];
	// clang-format on
	MyContentGrid->SetColumnFill(0, 1);

	MyButton->SetContent(MyContentGrid.ToSharedRef());
	MyButton->SetHAlign(HAlign_Fill);
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
	if (InputHintImage)
	{
		InputHintImage->SetKeyToShow(VirtualKey);
		InputHintImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

ESlateVisibility UModioRichTextButton::GetInputHintVisibility(EModioUIInputMode InputMode)
{
	if (InputMode == EModioUIInputMode::Mouse || InputMode == EModioUIInputMode::Unknown ||
		InputMode == EModioUIInputMode::Keyboard)
	{
		return ESlateVisibility::Collapsed;
	}
	else
	{
		return ESlateVisibility::SelfHitTestInvisible;
	}
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

void UModioRichTextButton::SetJustification(ETextJustify::Type NewJustification)
{
	Justification = NewJustification;
}
