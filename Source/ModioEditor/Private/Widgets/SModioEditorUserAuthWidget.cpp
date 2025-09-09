/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */
#include "Widgets/SModioEditorUserAuthWidget.h"
#include "Engine/Engine.h"
#include "Libraries/ModioErrorConditionLibrary.h"
#include "Libraries/ModioSDKLibrary.h"
#include "Misc/MessageDialog.h"
#include "ModioEditor.h"
#include "Widgets/Images/SThrobber.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SHyperlink.h"
#include "Widgets/Text/SRichTextBlock.h"
#include "Widgets/Text/STextBlock.h"
#include "WindowManager.h"

#define LOCTEXT_NAMESPACE "ModioEditorUserAuthWidget"

void SModioEditorUserAuthWidget::Construct(const FArguments& InArgs)
{
	ParentWindow = InArgs._ParentWindow;
	BackHandler = InArgs._BackHandler;

	// clang-format off
	ChildSlot
	[
		SNew(SBox)
		.MinDesiredWidth(900)
		.MinDesiredHeight(500)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.FillHeight(0.8f)
			.Padding(FMargin(15.f, 15.f, 15.f, 15.f))
			[
				SAssignNew(RootWidget, SVerticalBox)
			]

			+ SVerticalBox::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Bottom)
			[
				CreateTermsAndPrivacyLinks()
			]

			+ SVerticalBox::Slot()
			.Padding(FMargin(0, 12))
			.FillHeight(0.1f)
			.VAlign(VAlign_Bottom)
			[
				SNew(SSeparator)
				.SeparatorImage(ParentWindow->BoldSeperatorBrush)
				.Thickness(1.0f)
			]

			+ SVerticalBox::Slot()
			.VAlign(VAlign_Bottom)
			.Padding(2.5f)
			.FillHeight(0.2f)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Bottom)
				[
					SNew(SButton)
					.ContentPadding(ParentWindow->BottomButtonPadding)
					.Text(LOCTEXT("Back", "Back"))
					.OnClicked_Lambda([this]()
					{ 
						BackHandler.ExecuteIfBound();
						return FReply::Handled();
					})
					.IsEnabled_Lambda([this]() { return !bIsLoading; })
				]
			]
		]
	];
	// clang-format on

	DrawThrobberWidget();
	LoadModioSubsystem();
}

void SModioEditorUserAuthWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime,
									  const float InDeltaTime)
{
	if (ModioSubsystem && !ModioSubsystem->IsUsingBackgroundThread())
	{
		ModioSubsystem->RunPendingHandlers();
	}
}

void SModioEditorUserAuthWidget::UnloadResources()
{
	RootWidget = nullptr;
	ModioEmailEditableTextBox = nullptr;
	ModioAuthenticationCodeEditableTextBox = nullptr;
}

void SModioEditorUserAuthWidget::LoadModioSubsystem()
{
	FModioInitializeOptions InitializeOptions = UModioSDKLibrary::GetProjectInitializeOptions();

	if (GEngine)
	{
		ModioSubsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
		if (ModioSubsystem)
		{
			ModioSubsystem->InitializeAsync(InitializeOptions, FOnErrorOnlyDelegateFast::CreateRaw(
																   this, &SModioEditorUserAuthWidget::OnInitCallback));
		}
	}
}

void SModioEditorUserAuthWidget::OnInitCallback(FModioErrorCode ErrorCode)
{
	if (!ErrorCode.GetValue() ||
		UModioErrorConditionLibrary::ErrorCodeMatches(ErrorCode, EModioErrorCondition::SDKAlreadyInitialized))
	{
		UE_LOG(LogTemp, Display,
			   TEXT("ModioSubsystem - UserAuthWidget - OnInitCallback - ModioSubsystem initialized."));
		ModioSubsystem->VerifyUserAuthenticationAsync(
			FOnErrorOnlyDelegateFast::CreateRaw(this, &SModioEditorUserAuthWidget::OnUserAuthCheckResponse));
	}
	else
	{
		UE_LOG(LogTemp, Error,
			   TEXT("ModioSubsystem - UserAuthWidget - OnInitCallback - Could not initialize ModioSubsystem - "
					"ErrorCode: %d | "
					"ErrorMessage: %s"),
			   ErrorCode.GetValue(), *ErrorCode.GetErrorMessage());
		WindowManager::Get().CloseWindow();
	}
}

void SModioEditorUserAuthWidget::ClearAllWidgets()
{
	bIsLoading = false;

	if (RootWidget.IsValid())
	{
		if (RootWidget->GetAllChildren()->Num() > 0)
		{
			RootWidget->ClearChildren();
		}
	}
}

void SModioEditorUserAuthWidget::DrawThrobberWidget()
{
	ClearAllWidgets();

	bIsLoading = true;

	// clang-format off
	RootWidget->AddSlot()
		.Padding(FMargin(15.f, 15.f, 15.f, 15.f))
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(SCircularThrobber)
			.Radius(50.f)
		];
	// clang-format on
}

void SModioEditorUserAuthWidget::OnUserAuthCheckResponse(FModioErrorCode ErrorCode)
{
	if (ErrorCode.GetValue() == 0)
	{
		OnAuthenticationComplete.ExecuteIfBound(AuthenticationResult);
	}
	else
	{
		DrawLoginWidget();
	}
}

FReply SModioEditorUserAuthWidget::OnLoginLandingButtonClicked()
{
	DrawLoginWidget();
	return FReply::Handled();
}

void SModioEditorUserAuthWidget::DrawLoginWidget()
{
	ClearAllWidgets();

	// clang-format off
	
	RootWidget->AddSlot()
	.AutoHeight()
	.HAlign(HAlign_Fill)
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Center)
		.Padding(0, 0, 0, 0)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("EmailAuth", "Email Authentication"))
			.Font(ParentWindow->HeaderLargeTextStyle)
			.Justification(ETextJustify::Center)
		]
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.Padding(0, 0, 0, 16)
		[
			SNew(SBox)
			.VAlign(VAlign_Center)
			.MinDesiredWidth(700)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.AutoWidth()
				[
					SNew(STextBlock)
					.Justification(ETextJustify::Left)
					.Text(LOCTEXT("EnterEmail", "Enter your email address"))
				]
				+ SHorizontalBox::Slot()
				.Padding(8, 0)
				.FillWidth(0.9f)
				.VAlign(VAlign_Center)
				[
					SAssignNew(ModioEmailEditableTextBox, SEditableTextBox)
					.MinDesiredWidth(256.f)
				]
			]
		]
		
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.Padding(0, 0, 0, 16)
		[
			SNew(SBox)
			.MinDesiredWidth(500)
			.MinDesiredHeight(100)
			[
				SNew(SButton)
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					.OnClicked(this, &SModioEditorUserAuthWidget::OnLoginButtonClicked)
					.Text(LOCTEXT("SendCode", "Send Code"))
			]
		]
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(SButton)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			.ContentPadding(8)
			.OnClicked(this, &SModioEditorUserAuthWidget::OnAlreadyHaveCodeClicked)
			.Text(LOCTEXT("AlreadyHaveCode", "I already have a code"))

		]
	];

	// clang-format on
}

FReply SModioEditorUserAuthWidget::OnAlreadyHaveCodeClicked()
{
	DrawAuthenticateWidget();
	return FReply::Handled();
}

FReply SModioEditorUserAuthWidget::OnLoginButtonClicked()
{
	if (ModioEmailEditableTextBox->GetText().ToString().IsEmpty())
	{
		FText Message = LOCTEXT("ModioLoginInvalid", "Unable to login, please enter a valid email address for mod.io!");
		FMessageDialog::Open(EAppMsgType::Ok, Message);
		WindowManager::Get().GetWindow()->BringToFront();
		return FReply::Handled();
	}

	DrawThrobberWidget();

	ModioSubsystem->RequestEmailAuthCodeAsync(
		FModioEmailAddress(ModioEmailEditableTextBox->GetText().ToString()),
		FOnErrorOnlyDelegateFast::CreateRaw(this, &SModioEditorUserAuthWidget::OnRequestEmailAuthCodeCompleted));
	return FReply::Handled();
}

void SModioEditorUserAuthWidget::OnRequestEmailAuthCodeCompleted(FModioErrorCode ErrorCode)
{
	if (ErrorCode.GetValue() == 0)
	{
		DrawAuthenticateWidget();
	}
	else if (ErrorCode.GetValue() == 20993)
	{
		FText Message = FText::FromString("User already authenticated: " + ErrorCode.GetErrorMessage());
		FMessageDialog::Open(EAppMsgType::Ok, Message);
		OnAuthenticationComplete.ExecuteIfBound(ErrorCode);
	}
	else
	{
		DrawLoginWidget();

		FText Message = FText::FromString("Error, bad credentials: " + ErrorCode.GetErrorMessage());
		FMessageDialog::Open(EAppMsgType::Ok, Message);
		WindowManager::Get().GetWindow()->BringToFront();
	}
}

void SModioEditorUserAuthWidget::DrawAuthenticateWidget()
{
	ClearAllWidgets();

	// clang-format off
	FSlateFontInfo CodeFont = ParentWindow->GetTextStyle("EmbossedText", "Bold", 40);
	CodeFont.LetterSpacing = 75;

	// clang-format off
	RootWidget->AddSlot()
	.AutoHeight()
	.HAlign(HAlign_Fill)
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Center)
		.Padding(0, 0, 0, 32)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("EmailAuth", "Email Authentication"))
			.Font(ParentWindow->HeaderLargeTextStyle)
			.Justification(ETextJustify::Center)
		]
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Center)
		.AutoHeight()
		.Padding(0, 0, 0, 8)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("EnterCode", "Enter Authentication Code"))
			.Font(ParentWindow->GetTextStyle("EmbossedText", "Normal", 10))
			.Justification(ETextJustify::Center)
		]
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Center)
		.AutoHeight()
		.Padding(0, 0, 0, 0)
		[
			SNew(SBox)
			.MinDesiredWidth(300)
			.MinDesiredHeight(80)
			[
				SAssignNew(ModioAuthenticationCodeEditableTextBox, SEditableTextBox)
				.Font(CodeFont)
				.OnTextChanged(this, &SModioEditorUserAuthWidget::OnAuthCodeTextChanged)
				.Justification(ETextJustify::Center)
				.MinDesiredWidth(256.f)
			]
		]
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.Padding(24)
		.AutoHeight()
		[
			SNew(SBox)		
			.MinDesiredWidth(500)
			.MinDesiredHeight(50)
			[
				SNew(SButton)
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.ContentPadding(12)
				.OnClicked(this, &SModioEditorUserAuthWidget::OnAuthenticateButtonClicked)
				.Text(LOCTEXT("SubmitCode", "Submit Code"))
			]
		]
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.Padding(24)
		[
			SNew(SBox)		
			.MinDesiredWidth(300)
			.MinDesiredHeight(50)
			[
				SNew(SButton)
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.ContentPadding(8)
				.OnClicked(this, &SModioEditorUserAuthWidget::OnUseDifferentEmailClicked)
				.Text(LOCTEXT("DifferentEmail", "Enter a different email"))
			]

		]
	];
	// clang-format on
}

FReply SModioEditorUserAuthWidget::OnUseDifferentEmailClicked()
{
	DrawLoginWidget();
	return FReply::Handled();
}

FReply SModioEditorUserAuthWidget::OnAuthenticateButtonClicked()
{
	DrawThrobberWidget();

	ModioSubsystem->AuthenticateUserEmailAsync(
		FModioEmailAuthCode(ModioAuthenticationCodeEditableTextBox->GetText().ToString()),
		FOnErrorOnlyDelegateFast::CreateRaw(this, &SModioEditorUserAuthWidget::OnAuthCodeCompleted));
	return FReply::Handled();
}

void SModioEditorUserAuthWidget::OnAuthCodeTextChanged(const FText& InText) const
{
	ModioAuthenticationCodeEditableTextBox->SetText(FText::FromString(InText.ToString().Left(5).ToUpper()));
}

void SModioEditorUserAuthWidget::OnAuthCodeCompleted(FModioErrorCode ErrorCode)
{
	AuthenticationResult = ErrorCode;
	if (ErrorCode == 0)
	{
		ParentWindow->DrawToolLanding();
	}
	else
	{
		DrawAuthenticateWidget();

		FText Message = FText::FromString("Error: " + ErrorCode.GetErrorMessage());
		FMessageDialog::Open(EAppMsgType::Ok, Message);
		WindowManager::Get().GetWindow()->BringToFront();
	}
}

TSharedRef<SWidget> SModioEditorUserAuthWidget::CreateTermsAndPrivacyLinks()
{
	// clang-format off
	return SNew(SHorizontalBox) 
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(FMargin(15.f, 0.f, 15.f, 0.f))
		[
			SNew(SHyperlink)
			.Text(LOCTEXT("ModioTermsAndConditions", "Terms & Conditions"))
			.OnNavigate(this, &SModioEditorUserAuthWidget::LaunchTermsUrl)
		] 
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(FMargin(15.f, 0.f, 15.f, 0.f))
		[
			SNew(SHyperlink)
			.Text(LOCTEXT("ModioPrivacyPolicy", "Privacy Policy"))
			.OnNavigate(this, &SModioEditorUserAuthWidget::LaunchPrivacyUrl)
		];
	// clang-format on
}

void SModioEditorUserAuthWidget::LaunchTermsUrl()
{
	ModioSubsystem->GetTermsOfUseAsync(
		FOnGetTermsOfUseDelegateFast::CreateLambda([](FModioErrorCode ec, TOptional<FModioTerms> Terms) {
			if (ec)
			{
				UE_LOG(ModioEditor, Error, TEXT("Failed to fetch terms and conditions: %s"), *ec.GetErrorMessage())
				return;
			}

			if (!Terms.IsSet())
			{
				UE_LOG(ModioEditor, Error, TEXT("Terms were not set when fetching terms and conditions"))
				return;
			}

			FPlatformProcess::LaunchURL(*Terms.GetValue().TermsLink.URL, nullptr, nullptr);
		}));
}

void SModioEditorUserAuthWidget::LaunchPrivacyUrl()
{
	ModioSubsystem->GetTermsOfUseAsync(
		FOnGetTermsOfUseDelegateFast::CreateLambda([](FModioErrorCode ec, TOptional<FModioTerms> Terms) {
			if (ec)
			{
				UE_LOG(ModioEditor, Error, TEXT("Failed to fetch terms of use: %s"), *ec.GetErrorMessage())
				return;
			}

			if (!Terms.IsSet())
			{
				UE_LOG(ModioEditor, Error, TEXT("Terms were not set when fetching terms of use"))
				return;
			}

			FPlatformProcess::LaunchURL(*Terms.GetValue().PrivacyLink.URL, nullptr, nullptr);
		}));
}

#undef LOCTEXT_NAMESPACE
