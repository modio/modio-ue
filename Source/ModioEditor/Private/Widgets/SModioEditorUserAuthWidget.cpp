/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "Widgets/SModioEditorUserAuthWidget.h"

#include "Libraries/ModioSDKLibrary.h"
#include "WindowManager.h"

#include "EngineMinimal.h"

#include "Misc/MessageDialog.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/SRichTextBlock.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Images/SThrobber.h"

#define LOCTEXT_NAMESPACE "ModioEditorUserAuthWidget"

void SModioEditorUserAuthWidget::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Top)
		.Padding(FMargin(15.f, 15.f, 15.f, 15.f))
		[
			SNew(SRichTextBlock)
			.Text(LOCTEXT("Preamble", "Authenticate with mod.io via email.\n\rIf you have not made an account yet sign up at mod.io"))
		]
		+ SVerticalBox::Slot()
		.Padding(FMargin(15.f, 15.f, 15.f, 15.f))
		[
			SAssignNew(RootWidget, SVerticalBox)
		]
	];

	DrawThrobberWidget();
	LoadModioSubsystem();
}

void SModioEditorUserAuthWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
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
	if (ErrorCode.GetValue() == 0 || ErrorCode.GetValue() == 21769)
	{
		UE_LOG(LogTemp, Display, TEXT("ModioSubsystem - UserAuthWidget - OnInitCallback - ModioSubsystem initialized."));
		ModioSubsystem->VerifyUserAuthenticationAsync(FOnErrorOnlyDelegateFast::CreateRaw(this, &SModioEditorUserAuthWidget::OnUserAuthCheckResponse));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ModioSubsystem - UserAuthWidget - OnInitCallback - Could not initialize ModioSubsystem - ErrorCode: %d | " "ErrorMessage: %s"), ErrorCode.GetValue(), *ErrorCode.GetErrorMessage());
		WindowManager::Get().CloseWindow();
	}
}

void SModioEditorUserAuthWidget::ClearAllWidgets()
{
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

	RootWidget->AddSlot()
		.Padding(FMargin(15.f, 15.f, 15.f, 15.f))
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(SCircularThrobber)
			.Radius(50.f)
		];
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

void SModioEditorUserAuthWidget::DrawLoginWidget()
{
	ClearAllWidgets();

	RootWidget->AddSlot()
	.Padding(FMargin(15.f, 15.f, 15.f, 15.f))
	.HAlign(HAlign_Center)
	.VAlign(VAlign_Top)
	.AutoHeight()
	[
		SNew(SBorder)
		.BorderBackgroundColor(FColor::White)
		.Padding(FMargin(15.f, 15.f, 15.f, 15.f))
		[
			SNew(SHorizontalBox)
			// Modio Email Label
			+ SHorizontalBox::Slot()
			.Padding(FMargin(15.f, 0.f, 15.f, 0.f))
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Top)
			.AutoWidth()
			[
				SNew(STextBlock)
				.Text(LOCTEXT("ModioEmail", "Log in with Email:"))
			]
			// Modio Email EditableTextBox
			+ SHorizontalBox::Slot()
			.Padding(FMargin(15.f, 0.f, 15.f, 0.f))
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.AutoWidth()
			[
				SAssignNew(ModioEmailEditableTextBox, SEditableTextBox)
				.MinDesiredWidth(256.f)
			]
			// Login Button
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.AutoWidth()
			[
				SNew(SButton)
				.DesiredSizeScale(FVector2D(1.f, 1.f))
				.OnClicked(this, &SModioEditorUserAuthWidget::OnLoginButtonClicked)
				[
					// Login Button Text
					SNew(STextBlock)
					.Text(LOCTEXT("Login", "Login"))
					.Justification(ETextJustify::Center)
				]
			]
		]
	];
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
	ModioSubsystem->RequestEmailAuthCodeAsync(FModioEmailAddress(ModioEmailEditableTextBox->GetText().ToString()), 
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

	RootWidget->AddSlot()
		.Padding(FMargin(15.f, 15.f, 15.f, 5.f))
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Top)
		[
			SNew(STextBlock)
				.Text(LOCTEXT("AuthPromptText", "Please enter the authentication code sent to your email."))
		];


	RootWidget->AddSlot()
	.Padding(FMargin(15.f, 15.f, 15.f, 15.f))
	.AutoHeight()
	[
		SNew(SBorder)
		.BorderBackgroundColor(FColor::White)
		.Padding(FMargin(15.f, 15.f, 15.f, 15.f))
		[
			SNew(SHorizontalBox)

			// Modio Authentication  Label
			+ SHorizontalBox::Slot()
			.Padding(FMargin(15.f, 0.f, 15.f, 0.f))
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.AutoWidth()
			[
				SNew(STextBlock)
				.Text(LOCTEXT("AuthCode", "Authentication Code:"))
			]

			// Modio Authentication EditableTextBox
			+ SHorizontalBox::Slot()
			.Padding(FMargin(15.f, 0.f, 15.f, 0.f))
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.AutoWidth()
			[
				SAssignNew(ModioAuthenticationCodeEditableTextBox, SEditableTextBox)
				.MinDesiredWidth(256.f)
			]

			// Authenticate Button
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.AutoWidth()
			[
				SNew(SButton)
				.DesiredSizeScale(FVector2D(1.f, 1.f))
				.OnClicked(this, &SModioEditorUserAuthWidget::OnAuthenticateButtonClicked)
				[
					// Authenticate Button Text
					SNew(STextBlock)
					.Text(LOCTEXT("ModioAuth", "Authenticate"))
					.Justification(ETextJustify::Center)
				]
			]
		]
	];
}

FReply SModioEditorUserAuthWidget::OnAuthenticateButtonClicked()
{
	DrawThrobberWidget();

	ModioSubsystem->AuthenticateUserEmailAsync(
		FModioEmailAuthCode(ModioAuthenticationCodeEditableTextBox->GetText().ToString()),
		FOnErrorOnlyDelegateFast::CreateRaw(this, &SModioEditorUserAuthWidget::OnAuthCodeCompleted));
	return FReply::Handled();
}

void SModioEditorUserAuthWidget::OnAuthCodeCompleted(FModioErrorCode ErrorCode)
{
	AuthenticationResult = ErrorCode;
	if (ErrorCode == 0)
	{
		OnAuthenticationComplete.ExecuteIfBound(AuthenticationResult);
	}
	else
	{
		DrawLoginWidget();
	}
}

#undef LOCTEXT_NAMESPACE