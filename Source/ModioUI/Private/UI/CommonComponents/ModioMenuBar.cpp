// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/CommonComponents/ModioMenuBar.h"
#include "Core/Input/ModioInputKeys.h"
#include "GameFramework/InputSettings.h"
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
