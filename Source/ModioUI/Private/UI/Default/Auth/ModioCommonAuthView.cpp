/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Default/Auth/ModioCommonAuthView.h"

#include "ModioSubsystem.h"
#include "ModioUISubsystem.h"
#include "Components/WidgetSwitcherSlot.h"
#include "Internationalization/Culture.h"
#include "UI/Foundation/Base/ModioCommonWidgetSwitcher.h"
#include "UI/Foundation/Base/Auth/ModioCommonEmailAuthViewBase.h"
#include "UI/Foundation/Base/Auth/ModioCommonEmailAuthCodeViewBase.h"
#include "UI/Foundation/Base/Auth/ModioCommonEmailAuthLoadingViewBase.h"
#include "UI/Foundation/Base/Auth/ModioCommonTermsOfUseViewBase.h"
#include "UI/Default/Auth/ModioCommonEmailAuthView.h"
#include "UI/Default/Auth/ModioCommonEmailAuthCodeView.h"
#include "Types/ModioErrorCode.h"
#include "Types/ModioTerms.h"
#include "Types/ModioAuthenticationParams.h"

void UModioCommonAuthView::SetShowTermsOfView_Implementation(bool bShow)
{
	if (AuthSwitcher && TermsOfUseView && EmailAuthView && AuthSwitcher->GetActiveWidget() == TermsOfUseView)
	{
		AuthSwitcher->SetActiveWidget(bShow ? Cast<UWidget>(TermsOfUseView.Get()) : Cast<UWidget>(EmailAuthView.Get()));
	}
}

void UModioCommonAuthView::NativeOnActivated()
{
	if (AuthSwitcher)
	{
		AuthSwitcher->ClearChildren();

		if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
		{
			// English by default
			EModioLanguage CurrentLanguage = EModioLanguage::English;

			// Use GetCurrentLanguage delegate if available
			if (Subsystem->GetCurrentLanguage.IsBound())
			{
				CurrentLanguage = Subsystem->GetCurrentLanguage.Execute();
			}
			// Fallback on game settings if available
			else if (FInternationalization::IsAvailable())
			{
				CurrentLanguage = Subsystem->ConvertLanguageCodeToModio(FInternationalization::Get().GetCurrentLanguage()->GetTwoLetterISOLanguageName());
			}

			Subsystem->SetLanguage(CurrentLanguage);

			Subsystem->GetTermsOfUseAsync(FOnGetTermsOfUseDelegateFast::CreateWeakLambda(this, [this](FModioErrorCode ErrorCode, TOptional<FModioTerms> Terms) {
				if (!AuthSwitcher)
				{
					return;
				}

				TArray<UWidgetSwitcherSlot*> SwitcherSlots;
				if (TermsOfUseViewClass)
				{
					TermsOfUseView = CreateWidget<UModioCommonTermsOfUseViewBase>(this, TermsOfUseViewClass);
					TermsOfUseView->OnCancelClicked.AddUniqueDynamic(this, &UModioCommonAuthView::OnTermsOfUseViewCancelClicked);
					TermsOfUseView->OnSubmitClicked.AddUniqueDynamic(this, &UModioCommonAuthView::OnTermsOfUseViewSubmitClicked);
					TermsOfUseView->OnLinkClicked.AddUniqueDynamic(this, &UModioCommonAuthView::OnTermsOfUseViewLinkClicked);
					TermsOfUseView->Terms = Terms.Get(FModioTerms());
					TermsOfUseView->SynchronizeProperties();
					SwitcherSlots.Add(Cast<UWidgetSwitcherSlot>(AuthSwitcher->AddChild(TermsOfUseView)));
				}

				if (EmailAuthViewClass)
				{
					EmailAuthView = CreateWidget<UModioCommonEmailAuthViewBase>(this, EmailAuthViewClass);
					EmailAuthView->OnBackClicked.AddUniqueDynamic(this, &UModioCommonAuthView::OnEmailAuthViewBackClicked);
					EmailAuthView->OnCancelClicked.AddUniqueDynamic(this, &UModioCommonAuthView::OnEmailAuthViewCancelClicked);
					EmailAuthView->OnSubmitClicked.AddUniqueDynamic(this, &UModioCommonAuthView::OnEmailAuthViewSubmitClicked);
					SwitcherSlots.Add(Cast<UWidgetSwitcherSlot>(AuthSwitcher->AddChild(EmailAuthView)));
				}

				if (EmailAuthLoadingViewClass)
				{
					EmailAuthLoadingView = CreateWidget<UModioCommonEmailAuthLoadingViewBase>(this, EmailAuthLoadingViewClass);
					EmailAuthLoadingView->OnCancelClicked.AddUniqueDynamic(this, &UModioCommonAuthView::OnEmailAuthLoadingViewCancelClicked);
					SwitcherSlots.Add(Cast<UWidgetSwitcherSlot>(AuthSwitcher->AddChild(EmailAuthLoadingView)));
				}

				if (EmailAuthCodeViewBaseClass)
				{
					EmailAuthCodeView = CreateWidget<UModioCommonEmailAuthCodeViewBase>(this, EmailAuthCodeViewBaseClass);
					EmailAuthCodeView->OnBackClicked.AddUniqueDynamic(this, &UModioCommonAuthView::OnEmailAuthCodeViewBackClicked);
					EmailAuthCodeView->OnCancelClicked.AddUniqueDynamic(this, &UModioCommonAuthView::OnEmailAuthCodeViewCancelClicked);
					EmailAuthCodeView->OnSubmitClicked.AddUniqueDynamic(this, &UModioCommonAuthView::OnEmailAuthCodeViewSubmitClicked);
					SwitcherSlots.Add(Cast<UWidgetSwitcherSlot>(AuthSwitcher->AddChild(EmailAuthCodeView)));
				}

				for (UWidgetSwitcherSlot* SwitcherSlot : SwitcherSlots)
				{
					if (SwitcherSlot)
					{
						SwitcherSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
						SwitcherSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
					}
				}

				// Set the default active view as TermsOfUseView
				AuthSwitcher->SetActiveWidgetIndex(0);
			}));
		}
	}

	Super::NativeOnActivated();
}

void UModioCommonAuthView::NativeOnDeactivated()
{
	if (AuthSwitcher)
	{
		AuthSwitcher->SetActiveWidgetIndex(0);
		AuthSwitcher->ClearChildren();
	}
	Super::NativeOnDeactivated();
}

UWidget* UModioCommonAuthView::NativeGetDesiredFocusTarget() const
{
	if (AuthSwitcher && AuthSwitcher->GetChildrenCount() > 0)
	{
		if (UModioCommonActivatableWidget* ActivatableWidget = Cast<UModioCommonActivatableWidget>(AuthSwitcher->GetActiveWidget()))
		{
 			return ActivatableWidget->GetDesiredFocusTarget();
		}
	}

	return Super::NativeGetDesiredFocusTarget();
}

void UModioCommonAuthView::OnTermsOfUseViewSubmitClicked_Implementation()
{
	if (EmailAuthView)
	{
		AuthSwitcher->SetActiveWidget(EmailAuthView);
	}
}

void UModioCommonAuthView::OnTermsOfUseViewLinkClicked_Implementation(const FString& URL) 
{
	FPlatformProcess::LaunchURL(*URL, nullptr, nullptr);
}

void UModioCommonAuthView::OnTermsOfUseViewCancelClicked_Implementation()
{
	DeactivateWidget();
}

void UModioCommonAuthView::OnEmailAuthViewBackClicked_Implementation()
{
	if (TermsOfUseView)
	{
		AuthSwitcher->SetActiveWidget(TermsOfUseView);
	}
}

void UModioCommonAuthView::OnEmailAuthViewCancelClicked_Implementation()
{
	DeactivateWidget();
}

void UModioCommonAuthView::OnEmailAuthViewSubmitClicked_Implementation(const FString& EmailAddress)
{
	if(UModioCommonEmailAuthView* Ref = Cast<UModioCommonEmailAuthView>(EmailAuthView)) 
	{
		if (!Ref->IsEmailValid(EmailAddress)) 
		{
			return;
		}
	}

	bAuthCancelled = false;
	if (EmailAuthLoadingView)
	{
		AuthSwitcher->SetActiveWidget(EmailAuthLoadingView);
	}

	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->ClearUserDataAsync(FOnErrorOnlyDelegateFast::CreateWeakLambda(this, [this, Subsystem, EmailAddress](FModioErrorCode ErrorCode)
		{
			if (ErrorCode)
			{
				UE_LOG(LogTemp, Error, TEXT("[UModioCommonAuthView::OnEmailAuthViewSubmitClicked] Failed to clear user data. Error code: %s"), *ErrorCode.GetErrorMessage());
			}
			Subsystem->RequestEmailAuthCodeAsync(FModioEmailAddress(EmailAddress), FOnErrorOnlyDelegateFast::CreateUObject(this, &UModioCommonAuthView::HandleEmailAuthCodeSent));
		}));
	}
}

void UModioCommonAuthView::OnEmailAuthLoadingViewCancelClicked_Implementation()
{
	bAuthCancelled = true;
	if (EmailAuthView)
	{
		AuthSwitcher->SetActiveWidget(EmailAuthView);
	}
}

void UModioCommonAuthView::OnEmailAuthCodeViewBackClicked_Implementation()
{
	if (EmailAuthView)
	{
		AuthSwitcher->SetActiveWidget(EmailAuthView);
	}
}

void UModioCommonAuthView::OnEmailAuthCodeViewCancelClicked_Implementation()
{
	bAuthCancelled = true;
	DeactivateWidget();
}

void UModioCommonAuthView::OnEmailAuthCodeViewSubmitClicked_Implementation(const FString& AuthCode)
{
	UModioCommonEmailAuthCodeView* Ref = Cast<UModioCommonEmailAuthCodeView>(EmailAuthCodeView);
	if(Ref)
	{
		if (!Ref->IsCodeValid(AuthCode)) 
		{	
			return;
		}
	}

	bAuthCancelled = false;
	if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		Subsystem->RequestEmailAuthentication(FModioEmailAuthCode(AuthCode), FOnErrorOnlyDelegateFast::CreateUObject(this, &UModioCommonAuthView::HandleEmailAuthenticated));
		if (Ref) 
		{
			Ref->SetIsBusy(true);
		}
	}
}

void UModioCommonAuthView::HandleEmailAuthCodeSent_Implementation(FModioErrorCode ErrorCode)
{
	if (bAuthCancelled)
	{
		return;
	}

	if (ErrorCode)
	{
		if (EmailAuthView)
		{
			AuthSwitcher->SetActiveWidget(EmailAuthView);
		}
		UE_LOG(LogTemp, Error, TEXT("UModioCommonAuthView::HandleEmailAuthCodeSent. Error Code: %s"), *ErrorCode.GetErrorMessage());
		return;
	}

	if (EmailAuthCodeView)
	{
		AuthSwitcher->SetActiveWidget(EmailAuthCodeView);
	}
}

void UModioCommonAuthView::HandleEmailAuthenticated_Implementation(FModioErrorCode ErrorCode)
{
	UModioCommonEmailAuthCodeView* Ref = Cast<UModioCommonEmailAuthCodeView>(EmailAuthCodeView);
	if (Ref) 
	{
		Ref->SetIsBusy(false);
	}

	if (bAuthCancelled)
	{
		return;
	}

	if (ErrorCode)
	{
		if (Ref)
		{
			Ref->SetValidationTextVisibility(ESlateVisibility::Visible);
		}
		return;
	}

	OnEmailAuthSucceeded.ExecuteIfBound();

	DeactivateWidget();
}