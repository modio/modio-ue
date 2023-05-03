/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/CommonComponents/ModioMenuBar.h"
#include "Core/Input/ModioInputKeys.h"
#include "GameFramework/InputSettings.h"
#include "UI/Commands/ModioCommonUICommands.h"
#include "ModioUISubsystem.h"
#include "Settings/ModioUISettings.h"

void UModioMenuBar::NativeConstruct()
{
	Super::NativeConstruct();
}

void UModioMenuBar::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (SearchButton)
	{
		SearchButton->OnClicked.AddDynamic(this, &UModioMenuBar::OnSearchClicked);
		SearchButton->DisplayHintForInputHandler(FModioInputKeys::RefineSearch);
	}
	if (BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &UModioMenuBar::OnBackClicked);
	}
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		if (UserProfileButton)
		{
			UserProfileButton->NativeUserChanged(Subsystem->QueryUserProfile());
		}
	}
}

void UModioMenuBar::SynchronizeProperties()
{
	Super::SynchronizeProperties();
}

void UModioMenuBar::OnBackClicked()
{
	if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		Subsystem->SendCommandToBrowserUI(FModioInputKeys::Back, 0);
	}
}

void UModioMenuBar::OnSearchClicked()
{
	if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		Subsystem->SendCommandToBrowserUI(FModioInputKeys::RefineSearch, 0);
	}
}

void UModioMenuBar::SetSearchButtonVisibility(bool bVisible)
{
	if (SearchButton)
	{
		SearchButton->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UModioMenuBar::SetBackButtonVisibility(bool bVisible)
{
	if (BackButton)
	{
		BackButton->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UModioMenuBar::SetProfileButtonVisibility(bool bVisible)
{
	if (UserProfileButton)
	{
		UserProfileButton->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}
