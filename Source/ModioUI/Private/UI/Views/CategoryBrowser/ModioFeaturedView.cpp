/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/Views/CategoryBrowser/ModioFeaturedView.h"
#include "UI/Views/CategoryBrowser/ModioFeaturedModCarousel.h"
#include "Algo/Transform.h"
#include "Core/ModioModInfoUI.h"
#include "GameFramework/InputSettings.h"
#include "Engine/Engine.h"
#include "ModioUISubsystem.h"
#include "Settings/ModioUISettings.h"
#include "UI/CommonComponents/ModioMenu.h"
#include "UI/Dialogs/ModioDialogController.h"
#include "UI/CommonComponents/ModioErrorRetryWidget.h"
#include "UI/BaseWidgets/Slate/SModioTileView.h"
#include "UI/BaseWidgets/ModioGridPanel.h"
#include "UI/CommonComponents/ModioTabBar.h"
#include "UI/Views/ModDetails/ModioAsyncOpWrapperWidget.h"

void UModioFeaturedView::NativeBeginLoadExternalData()
{
	Super::NativeBeginLoadExternalData();
	for (const auto& Category : AdditionalCategories->GetDisplayedEntryWidgets())
	{
		UModioFeaturedCategory* RealCategory = Cast<UModioFeaturedCategory>(Category);
		if (RealCategory)
		{
			RealCategory->LoadExternalData();
		}
	}
}

void UModioFeaturedView::OnAdditionalCategoryCreated(UUserWidget& CategoryWidget)
{
	UModioFeaturedCategory* RealCategory = Cast<UModioFeaturedCategory>(&CategoryWidget);
	RealCategory->OnCategorySelectionChanged().AddUniqueDynamic(this, &UModioFeaturedView::CategorySelectionChanged);
	FOnGetSelectionIndex SelectionIndexDelegate;
	SelectionIndexDelegate.BindDynamic(this, &UModioFeaturedView::GetSelectionIndex);
	RealCategory->SetSelectionIndexDelegate(SelectionIndexDelegate);
}

void UModioFeaturedView::NativeConstruct()
{
	Super::NativeConstruct();
	AdditionalCategories->OnEntryWidgetGenerated().AddUObject(this, &UModioFeaturedView::OnAdditionalCategoryCreated);
	FCustomWidgetNavigationDelegate additionaNavDelegate;
	additionaNavDelegate.BindUFunction(this, "SetFocusToPrimaryCategory");
	AdditionalCategories->SetNavigationRuleCustom(EUINavigation::Up, additionaNavDelegate);
	if (FeaturedModCarousel)
	{
		FCustomWidgetNavigationDelegate primaryNavDelegate;
		primaryNavDelegate.BindUFunction(this, "SetFocusToAdditionalCategory");
		FeaturedModCarousel->SetNavigationRuleCustom(EUINavigation::Down, primaryNavDelegate);
	}

	// In case we get an error message we better keep this here to keep navigation functionality
	if (PrimaryCategoryGridPanel)
	{
		FCustomWidgetNavigationDelegate primaryNavDelegate;
		primaryNavDelegate.BindUFunction(this, "SetFocusToAdditionalCategory");
		PrimaryCategoryGridPanel->SetNavigationRuleCustom(EUINavigation::Down, primaryNavDelegate);
	}
}

void UModioFeaturedView::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	IModioUIModInfoReceiver::Register<UModioFeaturedView>(EModioUIModInfoEventType::ListAllMods);
	IModioUIUserChangedReceiver::Register<UModioFeaturedView>();

	if (PrimaryFeaturedCategoryLoader)
	{
		TScriptInterface<IModioUIAsyncOperationWidget> InterfaceWrapper =
			TScriptInterface<IModioUIAsyncOperationWidget>(this);
		IModioUIAsyncHandlerWidget::Execute_LinkAsyncOperationWidget(PrimaryFeaturedCategoryLoader, InterfaceWrapper);
	}

	if (FeaturedViewContent)
	{
		FeaturedViewContent->SetActualAsyncOperationWidget(TScriptInterface<IModioUIAsyncOperationWidget>(this));
	}

	FetchPrimaryCategoryMods();
	bShouldShowBackButton = false;
}

void UModioFeaturedView::NativeRequestOperationRetry()
{
	FetchPrimaryCategoryMods();
}

void UModioFeaturedView::FetchPrimaryCategoryMods()
{
	UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
	if (Subsystem)
	{
		if (const UModioUISettings* Settings = UModioUISettings::StaticClass()->GetDefaultObject<UModioUISettings>())
		{
			if (!Settings->BrowserCategoryConfiguration.IsNull())
			{
				if (UModioModBrowserParams* ModBrowserParams = Settings->BrowserCategoryConfiguration.LoadSynchronous())
				{
					FModioFilterParams Filter = FModioFilterParams()
					                            .WithTags(ModBrowserParams->PrimaryCategoryParams->Tags)
					                            .WithoutTags(ModBrowserParams->PrimaryCategoryParams->ExcludedTags)
					                            .SortBy(ModBrowserParams->PrimaryCategoryParams->SortField,
					                                    ModBrowserParams->PrimaryCategoryParams->Direction)
					                            .IndexedResults(0, ModBrowserParams->PrimaryCategoryParams->Count);
					Subsystem->RequestListAllMods(Filter, GetName().ToString());
				}
			}
		}
		else
		{
			Subsystem->RequestListAllMods(FModioFilterParams().IndexedResults(0, 5), GetName().ToString());
		}
		IModioUIAsyncOperationWidget::Execute_NotifyOperationState(this, EModioUIAsyncOperationWidgetState::InProgress);
	}
}


FNavigationReply UModioFeaturedView::NativeOnNavigation(const FGeometry& InGeometry,
                                                        const FNavigationEvent& InNavigationEvent)
{
	return Super::NativeOnNavigation(InGeometry, InNavigationEvent);
}

FReply UModioFeaturedView::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	UModioUI4Subsystem* UISubsystem = GEngine->GetEngineSubsystem<UModioUI4Subsystem>();

	if (IsValid(UISubsystem) && UISubsystem->IsAnyDialogOpen())
	{
		return FReply::Handled();
	}

	if (IsValid(UISubsystem) && !(UISubsystem->GetLastInputDevice() == EModioUIInputMode::Mouse))
	{
		if (UISubsystem->GetCurrentFocusTarget())
		{
			UISubsystem->GetCurrentFocusTarget()->SetKeyboardFocus();
		}
		else
		{
			SetFocusToPrimaryCategory();
		}
	}
	else
	{
		SetFocusToPrimaryCategory();
	}

	return FReply::Handled();
}

FReply UModioFeaturedView::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InPointerEvent)
{
	return FReply::Unhandled();
}

FReply UModioFeaturedView::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	return FReply::Unhandled();
}

void UModioFeaturedView::SetFocusToPrimaryCategory() 
{
	UModioUI4Subsystem* UISubsystem = GEngine->GetEngineSubsystem<UModioUI4Subsystem>();

	if (IsValid(UISubsystem) && UISubsystem->IsAnyDialogOpen())
	{
		return;
	}

	if (FeaturedModCarousel && bModsFound)
	{
		FeaturedModCarousel->SetFocusToCenterWidget();
	}
	else
	{
		ModioErrorWithRetryWidget->SetKeyboardFocus();
	}

}

void UModioFeaturedView::SetFocusToAdditionalCategory() 
{
	UModioUI4Subsystem* UISubsystem = GEngine->GetEngineSubsystem<UModioUI4Subsystem>();

	if (IsValid(UISubsystem) && UISubsystem->IsAnyDialogOpen())
	{
		return;
	}

	AdditionalCategories->NavigateToIndex(0);
	AdditionalCategories->SetSelectedIndex(0);
}

int32 UModioFeaturedView::GetSelectionIndex()
{
	return CurrentlySelectedCategoryColumn;
}

void UModioFeaturedView::CategorySelectionChanged(int32 Index, UModioFeaturedCategory* RealCategory)
{
	CurrentlySelectedCategoryColumn = Index;
}

void UModioFeaturedView::NativeOnListAllModsRequestCompleted(FString RequestIdentifier, FModioErrorCode ec,
                                                             TOptional<FModioModInfoList> List)
{
	IModioUIModInfoReceiver::NativeOnListAllModsRequestCompleted(RequestIdentifier, ec, List);

	if (RequestIdentifier == GetName().ToString())
	{
		if (!ec)
		{
			bModsFound = true;
			CachedFeaturedItems.Empty();
			Algo::Transform(List.GetValue().GetRawList(), CachedFeaturedItems, [](const FModioModInfo& In) {
				UModioModInfoUI* WrappedMod = NewObject<UModioModInfoUI>();
				WrappedMod->Underlying = In;
				return WrappedMod;
			});
			FeaturedModCarousel->GenerateWidgets(CachedFeaturedItems);
			FeaturedModCarousel->RePositionWidgets();
			FeaturedModCarousel->SetFocusToCenterWidget();
			IModioUIAsyncOperationWidget::Execute_NotifyOperationState(this,
			                                                           EModioUIAsyncOperationWidgetState::Success);
		}
		else
		{
			IModioUIAsyncOperationWidget::Execute_NotifyOperationState(this, EModioUIAsyncOperationWidgetState::Error);
			GEngine->GetEngineSubsystem<UModioUISubsystem>()->DisplayErrorDialog(ec);
			if (ModioErrorWithRetryWidget)
			{
				bModsFound = false;
				UModioUI4Subsystem* UISubsystem = GEngine->GetEngineSubsystem<UModioUI4Subsystem>();
				if (IsValid(UISubsystem) && !(UISubsystem->GetLastInputDevice() == EModioUIInputMode::Mouse))
				{
					ModioErrorWithRetryWidget->RetryButton->SetKeyboardFocus();
				}
			}
		}
	}
}

bool UModioFeaturedView::ShouldShowSearchButtonForMenu()
{
	return true;
}

void UModioFeaturedView::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	if (const UModioUISettings* Settings = UModioUISettings::StaticClass()->GetDefaultObject<UModioUISettings>())
	{
		if (!Settings->BrowserCategoryConfiguration.IsNull())
		{
			if (UModioModBrowserParams* ModBrowserParams = Settings->BrowserCategoryConfiguration.LoadSynchronous())
			{
				AdditionalCategories->SetListItems(ModBrowserParams->AdditionalCategoryParams);
				AdditionalCategories->SetSelectedIndex(0);
			}
		}
	}
}

// Regenerates mod tiles on log in / out to update subscribe button and subscription indicators
void UModioFeaturedView::NativeUserChanged(TOptional<FModioUser> NewUser)
{
	IModioUIUserChangedReceiver::NativeUserChanged(NewUser);
	FetchPrimaryCategoryMods();
	SynchronizeProperties();
	FeaturedModCarousel->SetFocusToCenterWidget();
	AdditionalCategories->RegenerateAllEntries();
}
