/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Default/QuickAccess/ModioCommonQuickAccessTabView.h"

#include "ModioUI.h"
#include "Core/ModioModCollectionEntryUI.h"
#include "UI/Default/ModBrowser/ModioCommonModBrowser.h"
#include "UI/Default/QuickAccess/ModioCommonQuickAccessTabViewStyle.h"
#include "UI/Foundation/Components/Button/ModioCommonButtonBase.h"
#include "UI/Foundation/Components/Border/ModioCommonBorder.h"
#include "UI/Foundation/Components/Text/TextBlock/ModioCommonTextBlock.h"
#include "UI/Foundation/Utilities/ModOperationTracker/ModioCommonModOperationTrackerUserWidget.h"
#include "UI/Settings/Params/ModioCommonQuickAccessParams.h"
#include "UI/Settings/ModioCommonUISettings.h"

void UModioCommonQuickAccessTabView::SetStyle(TSubclassOf<UModioCommonQuickAccessTabViewStyle> InStyle)
{
	if (InStyle && InStyle != ModioStyle)
	{
		ModioStyle = InStyle;
		SynchronizeProperties();
	}
}

UModioCommonQuickAccessTabView::UModioCommonQuickAccessTabView()
{
	bAutoBindInputAction = true;
}

void UModioCommonQuickAccessTabView::HandleOnCloseButtonClicked_Implementation()
{
	DeactivateWidget();
}

void UModioCommonQuickAccessTabView::HandleOnMainGameMenuButtonClicked_Implementation()
{
	if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		Subsystem->ExecuteOnModBrowserCloseRequestedDelegate();
	}
}

void UModioCommonQuickAccessTabView::HandleOnAuthButtonClicked_Implementation()
{
	if (!IsUserAuthenticated())
	{
		if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
		{
			Subsystem->ShowUserAuth();
			HandleOnCloseButtonClicked();
		}
	}
	else
	{
		if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
		{
			Subsystem->ShowLogoutDialog();
		}
	}
}

void UModioCommonQuickAccessTabView::HandleOnCollectionButtonClicked_Implementation()
{
	if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		if (UModioCommonModBrowser* ModBrowser = Cast<UModioCommonModBrowser>(Subsystem->GetModBrowserInstance()))
		{
			ModBrowser->ShowCollectionView();
			DeactivateWidget();
		}
	}
}

UWidget* UModioCommonQuickAccessTabView::NativeGetDesiredFocusTarget() const
{
	if (UWidget* WidgetToFocus = BP_GetDesiredFocusTarget())
	{
		return WidgetToFocus;
	}

	if (MyCollectionButton)
	{
		return MyCollectionButton;
		}
	if (MainGameMenuButton)
	{
		return MainGameMenuButton;
		}
	if (AuthButton)
	{
		return AuthButton;
		}

	return Super::NativeGetDesiredFocusTarget();
}

void UModioCommonQuickAccessTabView::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	IModioUIModManagementEventReceiver::Register<UModioCommonQuickAccessTabView>();
	IModioUISubscriptionsChangedReceiver::Register<UModioCommonQuickAccessTabView>();
	IModioUIUserChangedReceiver::Register<UModioCommonQuickAccessTabView>();

	if (OverlayBackgroundBorder)
	{
		OverlayBackgroundBorder->OnModioCommonBorderClicked.AddWeakLambda(this, [this]() {
			HandleOnCloseButtonClicked();
		});
	}

	if (const UModioCommonUISettings* UISettings = GetDefault<UModioCommonUISettings>())
	{
		if (MainGameMenuButton)
		{
				MainGameMenuButton->SetVisibility(UISettings->QuickAccessParams.bShowMainGameMenu ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
				if (UISettings->QuickAccessParams.bShowMainGameMenu) 
				{
					ListenForInputAction(MainGameMenuButton, UISettings->QuickAccessParams.MainGameMenuInputAction, UISettings->QuickAccessParams.MainGameMenuButtonLabel, FOnModioCommonActivatableWidgetActionFiredFast::CreateWeakLambda(this, [this]() {
						HandleOnMainGameMenuButtonClicked();
					}));
			}
		}

		if (AuthButton)
		{
			if(UISettings->bShowAuthButton)
			{
				ListenForInputAction(AuthButton, UISettings->QuickAccessParams.AuthInputAction, IsUserAuthenticated() ? UISettings->QuickAccessParams.LogOutButtonLabel : UISettings->QuickAccessParams.LogInButtonLabel, FOnModioCommonActivatableWidgetActionFiredFast::CreateWeakLambda(this, [this]() {
					HandleOnAuthButtonClicked();
				}));
			}
			else
			{
				AuthButton->SetVisibility(ESlateVisibility::Collapsed);
			}
		}

		if (MyCollectionButton)
		{
			ListenForInputAction(MyCollectionButton, UISettings->QuickAccessParams.MyCollectionInputAction, UISettings->QuickAccessParams.MyCollectionButtonLabel, FOnModioCommonActivatableWidgetActionFiredFast::CreateWeakLambda(this, [this]() {
				HandleOnCollectionButtonClicked();
			}));
		}
	}
}

void UModioCommonQuickAccessTabView::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
	if (TOptional<FModioModProgressInfo> CurrentProgress = Subsystem->QueryCurrentModUpdate())
	{
			//UpdateMods();
	}
}
}

void UModioCommonQuickAccessTabView::NativeUserChanged(TOptional<FModioUser> NewUser)
{
	Super::NativeUserChanged(NewUser);
	SynchronizeProperties();
}

void UModioCommonQuickAccessTabView::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		if (UserNameTextBlock)
		{
			TOptional<FModioUser> User = Subsystem->QueryUserProfile();
			if (User.IsSet())
			{
				UserNameTextBlock->SetText(FText::FromString(User.GetValue().Username));
			}
			else
			{
				// TODO
				UserNameTextBlock->SetText(NSLOCTEXT("Modio", "Guest", "Guest"));
			}
		}
	}

	if (const UModioCommonUISettings* UISettings = GetDefault<UModioCommonUISettings>()) 
	{
		if (MainGameMenuButton)
		{
			MainGameMenuButton->SetLabel(UISettings->QuickAccessParams.MainGameMenuButtonLabel);
		}

		if (AuthButton)
		{
			AuthButton->SetLabel(IsUserAuthenticated() ? UISettings->QuickAccessParams.LogOutButtonLabel : UISettings->QuickAccessParams.LogInButtonLabel);
		}

		if (MyCollectionButton)
		{
			MyCollectionButton->SetLabel(UISettings->QuickAccessParams.MyCollectionButtonLabel);
		}
	}
	
	if (UModioCommonQuickAccessTabViewStyle* StyleCDO = Cast<UModioCommonQuickAccessTabViewStyle>(ModioStyle.GetDefaultObject()))
	{
		if (InternalBackgroundBorder)
		{
			InternalBackgroundBorder->SetStyle(StyleCDO->InternalBackgroundBorderStyle);
		}

		if (OverlayBackgroundBorder)
		{
			OverlayBackgroundBorder->SetStyle(StyleCDO->OverlayBackgroundBorderStyle);
		}

		if (UserNameTextBlock)
		{
			UserNameTextBlock->SetStyle(StyleCDO->UserNameTextStyle);
		}

		if (ModOperationTrackerUserWidget)
		{
			ModOperationTrackerUserWidget->SetStyle(StyleCDO->ModOperationTrackerStyle);
		}

		if (MainGameMenuButton)
		{
			MainGameMenuButton->SetStyle(StyleCDO->MainGameMenuButtonStyle);
		}

		if (AuthButton)
		{
			AuthButton->SetStyle(StyleCDO->AuthButtonStyle);
		}

		if (MyCollectionButton)
		{
			MyCollectionButton->SetStyle(StyleCDO->MyCollectionButtonStyle);
		}
	}
}
