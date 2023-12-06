﻿/*
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
#include "UI/Default/Dialog/ModioCommonDialogInfo.h"
#include "UI/Default/ModBrowser/Featured/ModioCommonFeaturedView.h"
#include "UI/Settings/ModioCommonUISettings.h"
#include "Algo/Find.h"
#include "UI/Default/ModBrowser/Collection/ModioCommonCollectionView.h"
#include "UI/Foundation/Base/ModioCommonActivatableWidgetStack.h"

// These are only for the internal identification of the tabs
const FName FeaturedTabId = FName(TEXT("Featured"));
const FName CollectionTabId = FName(TEXT("Collection"));
const FName ModDetailsTabId = FName(TEXT("ModDetails"));

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

			const UModioCommonUISettings* UISettings = GetDefault<UModioCommonUISettings>();
			if (!UISettings)
			{
				UE_LOG(ModioUI5, Error, TEXT("Unable to process tab selection in '%s' for tab '%s': Settings are invalid"), *GetName(), *TabNameID.ToString());
				return;
			}

			TSubclassOf<UModioCommonActivatableWidget> SelectedViewClass;
			if (!GetViewFromTabNameID(TabNameID, SelectedViewClass))
			{
				UE_LOG(ModioUI5, Error, TEXT("Unable to process tab selection in '%s' for tab '%s': no associated view found"), *GetName(), *TabNameID.ToString());
				return;
			}

			HideQuickAccessView();
			HideSearchView();
			HideReportMod();
			HideModDetailsView();

			if (!ContentStack)
			{
				UE_LOG(ModioUI5, Error, TEXT("Unable to process tab selection in '%s' for tab '%s': content switcher is invalid"), *GetName(), *TabNameID.ToString());
				return;
			}

			UModioCommonActivatableWidget* SelectedView = ContentStack->AddWidgetSmart<UModioCommonActivatableWidget>(SelectedViewClass);
			UE_LOG(ModioUI5, Log, TEXT("Tab '%s' with view '%s' selected"), *TabNameID.ToString(), *SelectedViewClass->GetName());

			const FModioModCategoryParams* SelectedAdditionalCategoryParamsPtr = Algo::FindByPredicate(UISettings->FeaturedParams.CategoryParams,
			[this, TabNameID](const FModioModCategoryParams& Params) {
				return TabNameID == FName(*Params.CategoryName.ToString());
			});

			if (SelectedAdditionalCategoryParamsPtr)
			{
				if (UModioCommonFeaturedView* CastedFeaturedView = Cast<UModioCommonFeaturedView>(SelectedView))
				{
					UModioFilterParamsUI* FilterParamsUI = NewObject<UModioFilterParamsUI>();
					if (!FilterParamsUI)
					{
						UE_LOG(ModioUI5, Error, TEXT("Unable to show search results in '%s': failed to create filter params UI"), *GetName());
						return;
					}
					FilterParamsUI->Underlying = SelectedAdditionalCategoryParamsPtr->ToFilterParams();
					CastedFeaturedView->SetDataSource(FilterParamsUI);
				}
			}
		});

		auto CallHandleViewChanged = [this](UCommonActivatableWidgetContainerBase* Widget, bool bIsTransitioning) {
			if (!bIsTransitioning)
			{
				HandleViewChanged();
			}
		};

		ContentStack->OnTransitioningChanged.AddWeakLambda(this, CallHandleViewChanged);
		AuthStack->OnTransitioningChanged.AddWeakLambda(this, CallHandleViewChanged);
		RightTabStack->OnTransitioningChanged.AddWeakLambda(this, CallHandleViewChanged);
		DialogStack->OnTransitioningChanged.AddWeakLambda(this, CallHandleViewChanged);
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
	
	const UModioCommonUISettings* UISettings = GetDefault<UModioCommonUISettings>();
	if (!UISettings)
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

	if (ContentStack)
	{
		ContentStack->ClearWidgets();
	}

	ClearTabs();

	if (TabList)
	{
		TabList->SetPreviousTabInputActionData(UISettings->ModBrowserParams.PreviousTabInputAction);
		TabList->SetNextTabInputActionData(UISettings->ModBrowserParams.NextTabInputAction);
	}

	for (const FModioModCategoryParams& Params : UISettings->FeaturedParams.CategoryParams)
	{
		AddEmptyTab(FName(*Params.CategoryName.ToString()), Params.CategoryName);
	}

	if (!AddTab(CollectionTabId, UISettings->ModBrowserParams.CollectionViewTabText, StyleCDO->CollectionViewClass))
	{
		UE_LOG(ModioUI5, Warning, TEXT("Unable to synchronize some properties for '%s': unable to add Collection tab"), *GetName());
		return;
	}
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

	if (UCommonActivatableWidget* ActiveWidget = ContentStack->GetActiveWidget())
	{
		return ActiveWidget->GetDesiredFocusTarget();
	}

	return nullptr;
}

bool UModioCommonModBrowser::ShowFeaturedView_Implementation()
{
	if (!TabList)
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to show featured view in '%s': TabList is not bound"), *GetName());
		return false;
	}

	const UModioCommonUISettings* UISettings = GetDefault<UModioCommonUISettings>();
	if (!UISettings)
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to show featured view in '%s': Settings are invalid"), *GetName());
		return false;
	}

	// Selecting the first featured category tab
	for (const FModioModCategoryParams& Params : UISettings->FeaturedParams.CategoryParams)
	{
		FName TabName = FName(*Params.CategoryName.ToString());
		if (!TabList->SelectTabByID(TabName))
		{
			UE_LOG(ModioUI5, Error, TEXT("Unable to show featured view in '%s': TabList failed to select tab '%s'"), *GetName(), *TabName.ToString());
			return false;
		}
		return true;
	}

	UE_LOG(ModioUI5, Error, TEXT("Unable to show featured view in '%s': no featured category tabs found"), *GetName());
	return false;
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
	if (!RightTabStack->AddWidgetSmart<UModioCommonQuickAccessViewBase>(StyleCDO->QuickAccessViewClass))
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
	HandleViewChanged();
	return true;
}

bool UModioCommonModBrowser::ShowSearchView_Implementation(EModioCommonSearchViewType SearchType, const FModioModCategoryParams& CurrentFilterParams)
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
	UModioCommonSearchViewBase* SearchView = RightTabStack->AddWidgetSmart<UModioCommonSearchViewBase>(StyleCDO->SearchViewClass);
	if (!SearchView)
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to show search view in '%s': Unable to add '%s' to RightTabStack"), *GetName(), *StyleCDO->SearchViewClass->GetName());
		return false;
	}

	if (UModioSearchResultsParamsUI* SearchResultsParams = NewObject<UModioSearchResultsParamsUI>(this))
	{
		SearchResultsParams->SearchType = SearchType;
		SearchResultsParams->CurrentFilterParams = CurrentFilterParams;
		SearchView->SetDataSource(SearchResultsParams);
		return true;
	}

	UE_LOG(ModioUI5, Error, TEXT("Unable to show search view in '%s': Unable to create SearchResultsParams"), *GetName());
	return false;
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
	if (!ContentStack)
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable hide mod details: ContentSwitcher is not bound"));
		return false;
	}
	
	if (!TabList)
	{
		UE_LOG(ModioUI5, Warning, TEXT("Unable hide mod details: TabList is not bound"));
		return false;
	}
	
	if (UModioCommonModDetailsViewBase* ModDetailsView = Cast<UModioCommonModDetailsViewBase>(ContentStack->GetActiveWidget()))
	{
		ModDetailsView->DeactivateWidget();
	}
	return true;
}

bool UModioCommonModBrowser::AddTab_Implementation(FName TabNameID, const FText& TabText, TSubclassOf<UModioCommonActivatableWidget> ContentClass)
{
	if (!AddEmptyTab(TabNameID, TabText))
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to add tab '%s' (%s) in '%s': AddEmptyTab failed"), *TabNameID.ToString(), *TabText.ToString(), *GetName());
		return false;
	}

	UModioCommonActivatableWidget* AddedContent;
	if (!AddContentForTab(TabNameID, ContentClass, AddedContent))
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to add tab '%s' (%s) in '%s': AddContentForTab failed"), *TabNameID.ToString(), *TabText.ToString(), *GetName());
		return false;
	}
	return true;
}

bool UModioCommonModBrowser::AddEmptyTab_Implementation(FName TabNameID, const FText& TabText)
{
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

	return true;
}

bool UModioCommonModBrowser::AddContentForTab_Implementation(FName TabNameID, TSubclassOf<UModioCommonActivatableWidget> ContentClass, UModioCommonActivatableWidget*& OutContent)
{
	/*if (ContentClass)
	{
		OutContent = CreateWidget<UModioCommonActivatableWidget>(this, ContentClass);
		if (OutContent)
		{
			ContentStack->AddChild(OutContent);
			return true;
		}
	}*/
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

bool UModioCommonModBrowser::GetViewFromTabNameID_Implementation(FName TabNameID, TSubclassOf<UModioCommonActivatableWidget>& OutView) const
{
	const UModioCommonModBrowserStyle* StyleCDO = Cast<UModioCommonModBrowserStyle>(ModioStyle.GetDefaultObject());
	if (!StyleCDO)
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to synchronize properties for '%s': Style is invalid"), *GetName());
		return false;
	}
	
	if (TabNameID.IsEqual(FeaturedTabId))
	{
		OutView = StyleCDO->FeaturedViewClass;
		return true;
	}
	if (TabNameID.IsEqual(CollectionTabId))
	{
		OutView = StyleCDO->CollectionViewClass;
		return true;
	}
	if (TabNameID.IsEqual(ModDetailsTabId))
	{
		OutView = StyleCDO->ModDetailsClass;
		return true;
	}

	if (const UModioCommonUISettings* UISettings = GetDefault<UModioCommonUISettings>())
	{
		for (const FModioModCategoryParams& Params : UISettings->FeaturedParams.CategoryParams)
		{
			FName TabName = FName(*Params.CategoryName.ToString());
			if (TabNameID.IsEqual(TabName))
			{
				OutView = StyleCDO->FeaturedViewClass;
				return true;
			}
		}
	}

	UE_LOG(ModioUI5, Error, TEXT("Unable to get view from tab name ID '%s' in '%s': no associated view found"), *TabNameID.ToString(), *GetName());
	return false;
}

bool UModioCommonModBrowser::ShowModDetailsView_Implementation(const FModioModInfo& ModInfo)
{
	if (!ContentStack)
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to show mod details: content switcher is invalid"));
		return false;
	}

	const UModioCommonModBrowserStyle* StyleCDO = Cast<UModioCommonModBrowserStyle>(ModioStyle.GetDefaultObject());
	if (!StyleCDO)
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to show mod details: Style is invalid"));
		return false;
	}

	UModioCommonModDetailsViewBase* ModDetailsView = ContentStack->AddWidgetSmart<UModioCommonModDetailsViewBase>(StyleCDO->ModDetailsClass);
	if (!ModDetailsView)
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to show mod details: failed to create mod details view"));
		return false;
	}
	
	UModioModInfoUI* ModInfoObj = NewObject<UModioModInfoUI>();
	ModInfoObj->Underlying = ModInfo;
	ModDetailsView->SetDataSource(ModInfoObj);

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

void UModioCommonModBrowser::HandleViewChanged_Implementation()
{
	
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

void UModioCommonModBrowser::ShowSearchResults_Implementation(const FModioModCategoryParams& FilterParams)
{
	Super::ShowSearchResults_Implementation(FilterParams);

	if (UModioCommonFeaturedView* FeaturedWidget = Cast<UModioCommonFeaturedView>(ContentStack->GetActiveWidget()))
	{
		UModioFilterParamsUI* FilterParamsUI = NewObject<UModioFilterParamsUI>();
		if (!FilterParamsUI)
		{
			UE_LOG(ModioUI5, Error, TEXT("Unable to show search results in '%s': failed to create filter params UI"), *GetName());
			return;
		}
		FilterParamsUI->Underlying = FilterParams.ToFilterParams();
		FeaturedWidget->SetDataSource(FilterParamsUI);
	}
	else if (UModioCommonCollectionView* CollectionWidget = Cast<UModioCommonCollectionView>(ContentStack->GetActiveWidget()))
	{
		UModioModCategoryParamsUI* ModCategoryParamsUI = NewObject<UModioModCategoryParamsUI>();
		if (!ModCategoryParamsUI)
		{
			UE_LOG(ModioUI5, Error, TEXT("Unable to show search results in '%s': failed to create mod category params UI"), *GetName());
			return;
		}
		ModCategoryParamsUI->Underlying = FilterParams;
		CollectionWidget->SetDataSource(ModCategoryParamsUI);
	}
	else
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to show search results in '%s': active widget is not a collection or featured view"), *GetName());
	}

	HideModDetailsView();
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

bool UModioCommonModBrowser::GetIsCollectionModDisableUIEnabled_Implementation()
{
	if (const UModioCommonUISettings* UISettings = GetDefault<UModioCommonUISettings>())
	{
		return UISettings->bEnableCollectionModDisableUI;
	}
	return Super::GetIsCollectionModDisableUIEnabled_Implementation();
}

void UModioCommonModBrowser::ShowDialog_Implementation(UObject* DialogDataSource)
{
	UModioCommonDialogInfo* DialogInfo = Cast<UModioCommonDialogInfo>(DialogDataSource);
	if (!DialogInfo)
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to show dialog: DialogDataSource is not a DialogInfo"));
		return;
	}

	if (!DialogStack)
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to show dialog: DialogStack is not bound"));
		return;
	}

	const UModioCommonModBrowserStyle* StyleCDO = Cast<UModioCommonModBrowserStyle>(ModioStyle.GetDefaultObject());
	if (!StyleCDO)
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to show dialog for mod '%s': Style is invalid"), *DialogInfo->TitleText.ToString());
		return;
	}

	if (!StyleCDO->DialogClass)
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to show dialog for mod '%s': DialogClass in Style '%s' is invalid"), *DialogInfo->TitleText.ToString(), *ModioStyle->GetName());
		return;
	}

	DialogStack->ClearWidgets();

	if (UModioCommonDialogViewBase* DialogView = DialogStack->AddWidget<UModioCommonDialogViewBase>(StyleCDO->DialogClass))
	{
		DialogInfo->Owner = DialogView;
		DialogView->SetDataSource(DialogInfo);
	}

	HideQuickAccessView();
	HideSearchView();
}


#include "Loc/EndModioLocNamespace.h"