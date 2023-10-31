// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/BaseWidgets/ModioTermsLinksWidget.h"
#include "Core/ModioAuthenticationContextUI.h"
#include "UI/BaseWidgets/Slate/SModioButtonBase.h"
#include "UI/BaseWidgets/Slate/SModioRichTextBlock.h"
#include "UI/Styles/ModioButtonStyle.h"

#include "Loc/BeginModioLocNamespace.h"

TSharedRef<SWidget> UModioTermsLinksWidget::RebuildWidget()
{
	UModioAuthenticationContextUI* AuthContext = Cast<UModioAuthenticationContextUI>(DataSource);

	if (AuthContext != nullptr)
	{
		// clang-format off
		SAssignNew(ButtonList, SHorizontalBox)
		+SHorizontalBox::Slot()
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Center)
		.Padding(0, 0, 0, 0)
		[
			CreateButton(AuthContext->Terms.TermsLink.URL, FText::FromString(AuthContext->Terms.TermsLink.Text))
		]
		+ SHorizontalBox::Slot()
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Center)
		.Padding(0, 0, 0, 0)
		[
			CreateButton(AuthContext->Terms.PrivacyLink.URL, FText::FromString(AuthContext->Terms.PrivacyLink.Text))
		];

		// clang-format on

		return ButtonList.ToSharedRef();
	}
	else
	{
		// Log warning about not being able to fetch the terms links?
		return SAssignNew(ButtonList, SHorizontalBox);
	}
}

TSharedRef<SButton> UModioTermsLinksWidget::CreateButton(FString URL, FText ButtonTitle)
{
	TSharedPtr<SButton> Button;

	// clang-format off
	SAssignNew(Button, SModioButtonBase)
	.OnClicked_UObject(this, &UModioTermsLinksWidget::OnLinkClicked, URL)
	.HAlign(HAlign_Center)
	.VAlign(VAlign_Center)
	.ContentPadding(FMargin(16, 4,16, 2))
	[
		SNew(SModioRichTextBlock)
		.Text(ButtonTitle)
		.StyleReference_UObject(this, &UModioTermsLinksWidget::GetButtonTextStyle)
	];

	const FModioButtonStyle* ResolvedButtonStyle = ButtonStyle.FindStyle<FModioButtonStyle>();
	if (ResolvedButtonStyle)
	{
		Button->SetButtonStyle(&ResolvedButtonStyle->ButtonStyle);
		Button->SetContentPadding(ResolvedButtonStyle->ContentPadding);
	}
	Buttons.Add(Button);
	
	return Button.ToSharedRef();

}

FReply UModioTermsLinksWidget::OnLinkClicked(FString Link)
{
	FPlatformProcess::LaunchURL(*Link, nullptr, nullptr);
	return FReply::Handled();
}

void UModioTermsLinksWidget::NativeSetDataSource(UObject* InDataSource)
{
	DataSource = InDataSource;
}

void UModioTermsLinksWidget::NativeSetStyle(const FModioDialogStyle& Style)
{
	if (const FModioButtonStyle* ResolvedButtonStyle = Style.ButtonStyle.FindStyle<FModioButtonStyle>())
	{
		ButtonStyle = Style.ButtonStyle;

		for (TSharedPtr<SButton> CurrentButton : Buttons)
		{
			CurrentButton->SetButtonStyle(&ResolvedButtonStyle->ButtonStyle);
			CurrentButton->SetContentPadding(ResolvedButtonStyle->ContentPadding);

			CurrentButton->Invalidate(EInvalidateWidgetReason::Layout | EInvalidateWidgetReason::Paint);
		}
	}
}

void UModioTermsLinksWidget::NativeSetDialogController(class UModioDialogController* Controller)
{
	// Unimplemented
}


const FModioUIStyleRef* UModioTermsLinksWidget::GetButtonTextStyle() const
{
	if (const FModioButtonStyle* ResolvedButtonStyle = ButtonStyle.FindStyle<FModioButtonStyle>())
	{
		return &ResolvedButtonStyle->RichTextStyle;
	}
	
	return &FModioButtonStyle::GetDefault().RichTextStyle;
}


#include "Loc/EndModioLocNamespace.h"