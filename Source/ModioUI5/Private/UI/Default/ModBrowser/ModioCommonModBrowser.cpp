/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Default/ModBrowser/ModioCommonModBrowser.h"

#include "ModioUI5.h"
#include "Core/ModioFilterParamsUI.h"
#include "Core/ModioModInfoUI.h"
#include "UI/Default/ModBrowser/ModioCommonModBrowserStyle.h"
#include "UI/Foundation/Base/ModioCommonWidgetSwitcher.h"
#include "UI/Foundation/Base/Auth/ModioCommonAuthViewBase.h"
#include "UI/Foundation/Base/ModBrowser/ModioCommonModListBase.h"
#include "UI/Foundation/Base/ModDetails/ModioCommonModDetailsViewBase.h"
#include "UI/Foundation/Base/QuickAccess/ModioCommonQuickAccessViewBase.h"
#include "UI/Foundation/Base/Search/ModioCommonSearchViewBase.h"
#include "UI/Foundation/Components/Tab/ModioCommonTabListWidgetBase.h"
#include "UI/Settings/Params/ModioCommonModBrowserParams.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "UI/Foundation/Base/Report/ModioCommonReportViewBase.h"
#include "UI/Foundation/Base/Dialog/ModioCommonDialogViewBase.h"
#include "Loc/BeginModioLocNamespace.h"

// These are only for the internal identification of the tabs
const FName FeaturedTabId = FName(TEXT("Featured"));
const FName CollectionTabId = FName(TEXT("Collection"));
const FName SearchResultsTabId = FName(TEXT("SearchResults"));

void UModioCommonModBrowser::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
	if (!Subsystem)
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to initialize '%s': Modio UI Subsystem is invalid"), *GetName());
		return;
	}

	Subsystem->EnableModManagement();

	if (TabList)
	{
		TabList->OnTabSelectedFast.AddWeakLambda(this, [this](FName TabNameID) {
			if (!IsActivated())
			{
				return;
			}

			UModioCommonActivatableWidget* SelectedView;
			if (!GetViewFromTabNameID(TabNameID, SelectedView))
			{
				UE_LOG(ModioUI5, Error, TEXT("Unable to process tab selection in '%s' for tab '%s': no associated view found"), *GetName(), *TabNameID.ToString());
				return;
			}

			HideQuickAccessView();
			HideSearchView();
			HideReportMod();
			HideModDetailsView();

			if (!ContentSwitcher)
			{
				UE_LOG(ModioUI5, Error, TEXT("Unable to process tab selection in '%s' for tab '%s': content switcher is invalid"), *GetName(), *TabNameID.ToString());
				return;
			}
			
			ContentSwitcher->SetActiveWidget(SelectedView);
			UE_LOG(ModioUI5, Log, TEXT("Tab '%s' with view '%s' selected"), *TabNameID.ToString(), *SelectedView->GetName());
		});
	}
	else
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to fully initialize '%s': TabList is not bound"), *GetName());
	}
}

void UModioCommonModBrowser::NativeConstruct()
{
	Super::NativeConstruct();
	
	SynchronizeProperties();
	
	if (TabList)
	{
		TabList->SetListeningForInput(true);
	}
	else
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to fully construct '%s': TabList is not bound"), *GetName());
	}

	// Instantly create and hide the quick access view so that it's initialized and accumulates the list of downloading mods from the start
	{
		ShowQuickAccessView();
		HideQuickAccessView();
	}
}

void UModioCommonModBrowser::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	
	const UModioCommonModBrowserParamsSettings* Settings = GetDefault<UModioCommonModBrowserParamsSettings>();
	if (!Settings)
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to synchronize properties for '%s': Settings are invalid"), *GetName());
		return;
	}
	
	const UModioCommonModBrowserStyle* StyleCDO = Cast<UModioCommonModBrowserStyle>(ModioStyle.GetDefaultObject());
	if (!StyleCDO)
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to synchronize properties for '%s': Style is invalid"), *GetName());
		return;
	}

	if (ContentSwitcher)
	{
		ContentSwitcher->ClearChildren();
	}

	ClearTabs();

	TabList->SetPreviousTabInputActionData(Settings->PreviousTabInputAction);
	TabList->SetNextTabInputActionData(Settings->NextTabInputAction);

	if (ContentSwitcher && StyleCDO->ModDetailsClass)
	{
		ModDetailsView = CreateWidget<UModioCommonModDetailsViewBase>(this, StyleCDO->ModDetailsClass);
	}
	else
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to synchronize some properties for '%s': unable to create ModDetailsView"), *GetName());
	}

	UModioCommonActivatableWidget* FeaturedViewWidget;
	if (!AddTab(FeaturedTabId, Settings->FeaturedViewTabText, StyleCDO->FeaturedViewClass, FeaturedViewWidget))
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to synchronize some properties for '%s': unable to add Featured tab"), *GetName());
		return;
	}
	FeaturedView = Cast<UModioCommonModListBase>(FeaturedViewWidget);

	if (ContentSwitcher)
	{
		ContentSwitcher->AddChild(ModDetailsView);
	}

	UModioCommonActivatableWidget* CollectionViewWidget;
	if (!AddTab(CollectionTabId, Settings->CollectionViewTabText, StyleCDO->CollectionViewClass, CollectionViewWidget))
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to synchronize some properties for '%s': unable to add Collection tab"), *GetName());
		return;
	}
	CollectionView = Cast<UModioCommonModListBase>(CollectionViewWidget);

	UModioCommonActivatableWidget* SearchResultsViewWidget;
	if (!AddTab(SearchResultsTabId, Settings->SearchResultsViewTabText, StyleCDO->SearchResultsViewClass, SearchResultsViewWidget))
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to show search results in '%s': Failed to add tab '%s'"), *GetName(), *SearchResultsTabId.ToString());
		return;
	}
	SearchResultsView = Cast<UModioCommonModListBase>(SearchResultsViewWidget);

#if WITH_EDITOR
	if (IsDesignTime())
	{
		ContentSwitcher->SetActiveWidget(bPreviewDisplayFeaturedOrCollection ? Cast<UWidget>(FeaturedView) : Cast<UWidget>(CollectionView));
	}
#endif
}

UWidget* UModioCommonModBrowser::NativeGetDesiredFocusTarget() const
{
	if (UWidget* WidgetToFocus = BP_GetDesiredFocusTarget())
	{
		return WidgetToFocus;
	}

	if (!TabList)
	{
		UE_LOG(ModioUI5, Warning, TEXT("Unable to get desired focus target for '%s': TabList is not bound"), *GetName());
		return nullptr;
	}

	UModioCommonActivatableWidget* SelectedView;
	if (!GetViewFromTabNameID(TabList->GetActiveTab(), SelectedView))
	{
		UE_LOG(ModioUI5, Warning, TEXT("Unable to get desired focus target for '%s': no associated view with tab '%s' found"), *GetName(), *TabList->GetActiveTab().ToString());
		return nullptr;
	}

	return SelectedView->GetDesiredFocusTarget();
}

bool UModioCommonModBrowser::ShowFeaturedView_Implementation()
{
	if (!TabList)
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to show featured view in '%s': TabList is not bound"), *GetName());
		return false;
	}

	if (!TabList->SelectTabByID(FeaturedTabId))
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to show featured view in '%s': TabList failed to select tab '%s'"), *GetName(), *FeaturedTabId.ToString());
		return false;
	}

	return true;
}

bool UModioCommonModBrowser::HideFeaturedView_Implementation()
{
	// Featured view is only hidden when the collection view is shown, so we don't need to do anything here
	// (although users can override this behavior)
	return true;
}

bool UModioCommonModBrowser::ShowCollectionView_Implementation()
{
	if (!TabList)
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to show collection view in '%s': TabList is not bound"), *GetName());
		return false;
	}

	HideModDetailsView();

	if (!TabList->SelectTabByID(CollectionTabId))
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to show collection view in '%s': TabList failed to select tab '%s'"), *GetName(), *CollectionTabId.ToString());
		return false;
	}

	return true;
}

bool UModioCommonModBrowser::HideCollectionView_Implementation()
{
	// Collection view is only hidden when the featured view is shown, so we don't need to do anything here
	// (although users can override this behavior)
	return true;
}

bool UModioCommonModBrowser::ShowQuickAccessView_Implementation()
{
	HideSearchView();
	HideReportMod();

	if (!RightTabStack)
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to show quick access view in '%s': RightTabStack is invalid"), *GetName());
		return false;
	}

	const UModioCommonModBrowserStyle* StyleCDO = Cast<UModioCommonModBrowserStyle>(ModioStyle.GetDefaultObject());
	if (!StyleCDO)
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to show quick access view in '%s': Style is invalid"), *GetName());
		return false;
	}

	if (!StyleCDO->QuickAccessViewClass)
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to show quick access view in '%s': QuickAccessViewClass in Style '%s' is invalid"), *GetName(), *ModioStyle->GetName());
		return false;
	}

	RightTabStack->ClearWidgets();
	if (!RightTabStack->AddWidget<UModioCommonQuickAccessViewBase>(StyleCDO->QuickAccessViewClass))
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to show quick access view in '%s': Unable to add '%s' to RightTabStack"), *GetName(), *StyleCDO->QuickAccessViewClass->GetName());
		return false;
	}

	return true;
}

bool UModioCommonModBrowser::HideQuickAccessView_Implementation()
{
	if (!RightTabStack)
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to hide quick access view in '%s': RightTabStack is not bound"), *GetName());
		return false;
	}

	RightTabStack->ClearWidgets();
	return true;
}

bool UModioCommonModBrowser::ShowSearchView_Implementation()
{
	if (!RightTabStack)
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to show search view in '%s': RightTabStack is invalid"), *GetName());
		return false;
	}
	
	const UModioCommonModBrowserStyle* StyleCDO = Cast<UModioCommonModBrowserStyle>(ModioStyle.GetDefaultObject());
	if (!StyleCDO)
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to show search view in '%s': Style is invalid"), *GetName());
		return false;
	}

	if (!StyleCDO->SearchViewClass)
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to show search view in '%s': SearchViewClass in Style '%s' is invalid"), *GetName(), *ModioStyle->GetName());
		return false;
	}

	RightTabStack->ClearWidgets();
	if (!RightTabStack->AddWidget<UModioCommonSearchViewBase>(StyleCDO->SearchViewClass))
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to show search view in '%s': Unable to add '%s' to RightTabStack"), *GetName(), *StyleCDO->SearchViewClass->GetName());
		return false;
	}

	return true;
}

bool UModioCommonModBrowser::HideSearchView_Implementation()
{
	if (!RightTabStack)
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to hide search view in '%s': RightTabStack is not bound"), *GetName());
		return false;
	}

	RightTabStack->ClearWidgets();
	return true;
}

bool UModioCommonModBrowser::HideModDetailsView_Implementation()
{
	if (!ContentSwitcher)
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable hide mod details: ContentSwitcher is not bound"));
		return false;
	}
	
	if (!TabList)
	{
		UE_LOG(ModioUI5, Warning, TEXT("Unable hide mod details: TabList is not bound"));
		return false;
	}

	UModioCommonActivatableWidget* SelectedView;
	if (!GetViewFromTabNameID(TabList->GetActiveTab(), SelectedView))
	{
		UE_LOG(ModioUI5, Warning, TEXT("Unable hide mod details: no associated view found"));
		return false;
	}
	ContentSwitcher->SetActiveWidget(SelectedView);
	return true;
}

bool UModioCommonModBrowser::AddTab_Implementation(FName TabNameID, const FText& TabText, TSubclassOf<UModioCommonActivatableWidget> ContentClass, UModioCommonActivatableWidget*& OutContent)
{
	if (!ContentSwitcher)
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to add tab '%s' (%s) in '%s': ContentSwitcher is not bound"), *TabNameID.ToString(), *TabText.ToString(), *GetName());
		return false;
	}

	if (!TabList)
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to add tab '%s' (%s) in '%s': TabList is not bound"), *TabNameID.ToString(), *TabText.ToString(), *GetName());
		return false;
	}
	
	const UModioCommonModBrowserStyle* StyleCDO = Cast<UModioCommonModBrowserStyle>(ModioStyle.GetDefaultObject());
	if (!StyleCDO)
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to add tab '%s' (%s) in '%s': Style is invalid"), *TabNameID.ToString(), *TabText.ToString(), *GetName());
		return false;
	}

	if (!StyleCDO->TabButtonClass)
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to add tab '%s' (%s) in '%s': TabButtonClass in Style '%s' is invalid"), *TabNameID.ToString(), *TabText.ToString(), *GetName(), *ModioStyle->GetName());
		return false;
	}

	FModioCommonTabDescriptor TabInfo;
	TabInfo.TabId = TabNameID;
	TabInfo.TabText = TabText;
	TabInfo.TabButtonType = StyleCDO->TabButtonClass;
	TabInfo.TabButtonStyle = StyleCDO->TabButtonStyle;
	if (!TabList->RegisterDynamicTab(TabInfo))
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to add tab '%s' in '%s': TabList failed to register tab"), *TabNameID.ToString(), *GetName());
		return false;
	}

	OutContent = CreateWidget<UModioCommonActivatableWidget>(this, ContentClass);
	ContentSwitcher->AddChild(OutContent);
	return true;
}

void UModioCommonModBrowser::RemoveTab_Implementation(FName TabNameID)
{
	if (TabList)
	{
		TabList->RemoveTab(TabNameID);
	}
	else
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to remove tab '%s' in '%s': TabList is not bound"), *TabNameID.ToString(), *GetName());
	}
}

void UModioCommonModBrowser::ClearTabs_Implementation()
{
	if (TabList)
	{
		TabList->RemoveAllDynamicTabs();
	}
	else
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to clear tabs in '%s': TabList is not bound"), *GetName());
	}
}

bool UModioCommonModBrowser::GetViewFromTabNameID_Implementation(FName TabNameID, UModioCommonActivatableWidget*& OutView) const
{
	if (FeaturedView && TabNameID.IsEqual(FeaturedTabId))
	{
		OutView = FeaturedView;
		return true;
	}
	if (CollectionView && TabNameID.IsEqual(CollectionTabId))
	{
		OutView = CollectionView;
		return true;
	}
	if (SearchResultsView && TabNameID.IsEqual(SearchResultsTabId))
	{
		OutView = SearchResultsView;
		return true;
	}

	UE_LOG(ModioUI5, Error, TEXT("Unable to get view from tab name ID '%s' in '%s': no associated view found"), *TabNameID.ToString(), *GetName());
	return false;
}

bool UModioCommonModBrowser::ShowModDetailsView_Implementation(const FModioModInfo& ModInfo)
{
	if (!ContentSwitcher)
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to show mod details: content switcher is invalid"));
		return false;
	}

	if (!ModDetailsView)
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to show mod details: ModDetailsView is invalid"));
		return false;
	}
			
	UModioModInfoUI* ModInfoObj = NewObject<UModioModInfoUI>();
	ModInfoObj->Underlying = ModInfo;
	ModDetailsView->SetDataSource(ModInfoObj);
	ContentSwitcher->SetActiveWidget(ModDetailsView);

	ModDetailsView->OnDeactivated().AddWeakLambda(this, [this]() {
		HideModDetailsView();
	});

	HideQuickAccessView();
	HideSearchView();
	HideReportMod();
	
	return true;
}

bool UModioCommonModBrowser::HideReportMod_Implementation()
{
	if (!ReportStack)
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to hide report mod view in '%s': ReportStack is not bound"), *GetName());
		return false;
	}

	ReportStack->ClearWidgets();
	return true;
}

void UModioCommonModBrowser::ShowDetailsForMod_Implementation(FModioModID ModID)
{
	UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
	if (!Subsystem)
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to show details for mod '%s': Modio Subsystem is invalid"), *ModID.ToString());
		return;
	}

	Subsystem->GetModInfoAsync(ModID, FOnGetModInfoDelegateFast::CreateWeakLambda(this, [this](FModioErrorCode ErrorCode, TOptional<FModioModInfo> ModInfo) {
		if (ErrorCode)
		{
			UE_LOG(ModioUI5, Error, TEXT("Unable to show mod details, see error message below:\n\n%s"), *ErrorCode.GetErrorMessage());
			if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
			{
				Subsystem->DisplayErrorDialog(ErrorCode);
				return;
			}
			return;
		}

		if (!ModInfo.IsSet())
		{
			UE_LOG(ModioUI5, Error, TEXT("Unable to show mod details: mod info is not set"));
			return;
		}

		ShowModDetailsView(ModInfo.GetValue());
	}));
}

void UModioCommonModBrowser::ShowUserAuth_Implementation()
{
	if (const UModioCommonModBrowserStyle* StyleCDO = Cast<UModioCommonModBrowserStyle>(ModioStyle.GetDefaultObject()))
	{
		if (!StyleCDO->UserAuthClass || !AuthStack)
		{
			return;
		}
		UModioCommonAuthViewBase* AuthView = AuthStack->AddWidget<UModioCommonAuthViewBase>(StyleCDO->UserAuthClass);
	}
}

void UModioCommonModBrowser::LogOut_Implementation()
{
	if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		Subsystem->LogOut(FOnErrorOnlyDelegateFast::CreateWeakLambda(this,
			[this](FModioErrorCode ErrorCode) {
				if (ErrorCode)
				{
					UE_LOG(ModioUI5, Error, TEXT("Unable to log out, see error message below:\n\n%s"), *ErrorCode.GetErrorMessage());
				}
			}));
	}
}

void UModioCommonModBrowser::ShowSearchResults_Implementation(const FModioFilterParams& FilterParams)
{
	Super::ShowSearchResults_Implementation(FilterParams);

	const UModioCommonModBrowserParamsSettings* Settings = GetDefault<UModioCommonModBrowserParamsSettings>();
	if (!Settings)
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to show search results in '%s': Settings are invalid"), *GetName());
		return;
	}
	
	const UModioCommonModBrowserStyle* StyleCDO = Cast<UModioCommonModBrowserStyle>(ModioStyle.GetDefaultObject());
	if (!StyleCDO)
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to show search results in '%s': Style is invalid"), *GetName());
		return;
	}

	if (!StyleCDO->SearchResultsViewClass)
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to show search results in '%s': SearchResultsViewClass in Style '%s' is invalid"), *GetName(), *ModioStyle->GetName());
		return;
	}

	HideModDetailsView();

	UModioCommonActivatableWidget* SearchResultsViewWidget;
	if (!GetViewFromTabNameID(SearchResultsTabId, SearchResultsViewWidget))
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to show search results in '%s' from tab '%s': no associated view found"), *GetName(), *SearchResultsTabId.ToString());
		return;
	}

	if (UModioCommonModListBase* SearchResultsModListView = Cast<UModioCommonModListBase>(SearchResultsViewWidget))
	{
		UModioFilterParamsUI* FilterParamsUI = NewObject<UModioFilterParamsUI>();
		FilterParamsUI->Underlying = FilterParams;
		SearchResultsModListView->SetDataSource(FilterParamsUI);
	}
	else
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to show search results in '%s' from tab '%s': associated view ('%s') is not a mod list"), *GetName(), *SearchResultsTabId.ToString(), *SearchResultsViewWidget->GetName());
		return;
	}

	if (TabList)
	{
		TabList->SelectTabByID(SearchResultsTabId);
	}
}

void UModioCommonModBrowser::ShowReportMod_Implementation(UObject* DialogDataSource)
{
	Super::ShowReportMod_Implementation(DialogDataSource);

	if (!ReportStack)
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to show report: ReportStack is not bound"));
		return;
	}

	const UModioCommonModBrowserStyle* StyleCDO = Cast<UModioCommonModBrowserStyle>(ModioStyle.GetDefaultObject());
	if (!StyleCDO)
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to show report for mod '%s': Style is invalid"), *DialogDataSource->GetName());
		return;
	}

	if (!StyleCDO->ReportClass)
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to show report for mod '%s': ReportClass in Style '%s' is invalid"), *DialogDataSource->GetName(), *ModioStyle->GetName());
		return;
	}

	UModioCommonReportViewBase* ReportView = ReportStack->AddWidget<UModioCommonReportViewBase>(StyleCDO->ReportClass);
	ReportView->SetDataSource(DialogDataSource);

	HideQuickAccessView();
	HideSearchView();
}

void UModioCommonModBrowser::ShowDialog_Implementation(FModioModInfo ModInfo)
{
	Super::ShowDialog_Implementation(ModInfo);

	if (!DialogStack)
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to show dialog: DialogStack is not bound"));
		return;
	}

	const UModioCommonModBrowserStyle* StyleCDO = Cast<UModioCommonModBrowserStyle>(ModioStyle.GetDefaultObject());
	if (!StyleCDO)
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to show dialog for mod '%s': Style is invalid"), *ModInfo.ProfileName);
		return;
	}

	if (!StyleCDO->DialogClass)
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to show dialog for mod '%s': DialogClass in Style '%s' is invalid"),
			   *ModInfo.ProfileName, *ModioStyle->GetName());
		return;
	}

	UModioCommonDialogViewBase* DialogView = DialogStack->AddWidget<UModioCommonDialogViewBase>(StyleCDO->DialogClass);
	UModioModInfoUI* ModInfoObj = NewObject<UModioModInfoUI>();
	ModInfoObj->Underlying = ModInfo;
	DialogView->SetDataSource(ModInfoObj);

	HideQuickAccessView();
	HideSearchView();
}


#include "Loc/EndModioLocNamespace.h"