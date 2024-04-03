/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Foundation/Components/List/ModioCommonFilteredModListView.h"
#include "ModioUI.h"
#include "Components/ListView.h"
#include "Core/ModioFilterParamsUI.h"
#include "UI/Default/ErrorWithRetry/ModioCommonErrorWithRetryWidget.h"
#include "UI/Foundation/Components/Button/ModioCommonButtonBase.h"
#include "UI/Foundation/Components/List/ModioCommonFilteredModListViewStyle.h"
#include "UI/Foundation/Components/List/ModioCommonListView.h"
#include "UI/Foundation/Components/List/ModioCommonModTileView.h"
#include "UI/Foundation/Components/Text/TextBlock/ModioCommonTextBlock.h"
#include "UI/Interfaces/IModioUIAsyncHandlerWidget.h"
#include "UI/Settings/ModioCommonUISettings.h"
#include "UI/Settings/Params/ModioCommonFilteredModListParams.h"
#include "Containers/Ticker.h"

void UModioCommonFilteredModListView::SetStyle(TSubclassOf<UModioCommonFilteredModListViewStyle> InStyle)
{
	if (InStyle && InStyle != ModioStyle)
	{
		ModioStyle = InStyle;
		SynchronizeProperties();
	}
}

int32 UModioCommonFilteredModListView::GetNumItemsSelected_Implementation() const
{
	if (ModList)
	{
		return ModList->GetNumItemsSelected();
	}
	UE_LOG(ModioUI, Error, TEXT("Unable to get number of selected items in '%s': Mod list is invalid"), *GetName());
	return 0;
}

int32 UModioCommonFilteredModListView::GetNumItems_Implementation() const
{
	if (ModList)
	{
		return ModList->GetNumItems();
	}
	UE_LOG(ModioUI, Error, TEXT("Unable to get number of items in '%s': Mod list is invalid"), *GetName());
	return 0;
}

UWidget* UModioCommonFilteredModListView::NativeGetDesiredFocusTarget() const
{
	if (UWidget* WidgetToFocus = BP_GetDesiredFocusTarget())
	{
		return WidgetToFocus;
	}
	if (ModListLoader && ModListLoader->Implements<UModioUIAsyncHandlerWidget>()
		&& IModioUIAsyncHandlerWidget::Execute_GetAsyncOperationState(ModListLoader) == EModioUIAsyncOperationWidgetState::Error)
	{
		if (UWidget* WidgetToFocus = ErrorWithRetryWidget->GetDesiredFocusTarget())
		{
			return WidgetToFocus;
		}
	}
	if (ModList)
	{
		if (ModList->Implements<UModioCommonModListViewInterface>())
		{
			return IModioCommonModListViewInterface::Execute_GetDesiredListFocusTarget(ModList, true, true);
		}
		return ModList;
	}
	return Super::NativeGetDesiredFocusTarget();
}

void UModioCommonFilteredModListView::NativeOnSetDataSource()
{
	Super::NativeOnSetDataSource();

	UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
	if (!Subsystem)
	{
		UE_LOG(ModioUI, Error, TEXT("Unable to request filtered mod list in '%s': Modio UI Subsystem is invalid"), *GetName());
		if (ErrorWithRetryWidget)
		{
			if (UModioCommonErrorWithRetryParamsUI* ErrorWithRetryParams = NewObject<UModioCommonErrorWithRetryParamsUI>())
			{
				ErrorWithRetryParams->ErrorDescriptionTextLabel = NSLOCTEXT("Modio", "InvalidSubsystem", "Modio UI Subsystem is invalid");
				ErrorWithRetryWidget->SetDataSource(ErrorWithRetryParams);
			}
		}
		IModioUIAsyncOperationWidget::Execute_NotifyOperationState(this, EModioUIAsyncOperationWidgetState::Error);
		FocusOnDesiredWidget();
		return;
	}

	UModioFilterParamsUI* FilterParamsPtr = Cast<UModioFilterParamsUI>(DataSource);
	if (!FilterParamsPtr)
	{
		UE_LOG(ModioUI, Error, TEXT("Unable to request filtered mod list in '%s': Filter params are invalid"), *GetName());
		if (ErrorWithRetryWidget)
		{
			if (UModioCommonErrorWithRetryParamsUI* ErrorWithRetryParams = NewObject<UModioCommonErrorWithRetryParamsUI>())
			{
				ErrorWithRetryParams->ErrorDescriptionTextLabel = NSLOCTEXT("Modio", "InvalidFilterParams", "Filter params are invalid");
				ErrorWithRetryWidget->SetDataSource(ErrorWithRetryParams);
			}
		}
		IModioUIAsyncOperationWidget::Execute_NotifyOperationState(this, EModioUIAsyncOperationWidgetState::Error);
		FocusOnDesiredWidget();
		return;
	}

	ClearListeningInputActions();

	const bool bFocusOnceListIsPopulated = GetNumItemsSelected() > 0;
	if (Implements<UModioCommonModListViewInterface>())
	{
		IModioCommonModListViewInterface::Execute_RequestFullClearSelection(this, true);
		if (bFocusOnceListIsPopulated)
		{
			IModioCommonModListViewInterface::Execute_SetFocusOnceListIsPopulated(this, bFocusOnceListIsPopulated);
		}
	}
	if (ModList)
	{
		ModList->ClearSelection();
		ModList->RequestRefresh();
		ModList->ClearListItems();
	}
	
	SetPageNavigationVisibility(false);
	SetInitialScreenVisibility(false);
	SetNoResultsVisibility(false);
	IModioUIAsyncOperationWidget::Execute_NotifyOperationState(this, EModioUIAsyncOperationWidgetState::InProgress);

	OnSetModsFromModInfoListStarted.Broadcast();
	OnSetModsFromModInfoListDynamicStarted.Broadcast();

	const FModioFilterParams& FilterParams = FilterParamsPtr->Underlying;
	Subsystem->RequestListAllMods(FilterParams, GetRequestIdentifier());
}

FString UModioCommonFilteredModListView::GetRequestIdentifier_Implementation()
{
	UModioFilterParamsUI* FilterParamsPtr = Cast<UModioFilterParamsUI>(DataSource);
	if (!FilterParamsPtr)
	{
		UE_LOG(ModioUI, Error, TEXT("Unable to get request identifier for '%s': Filter params are invalid"),
			   *GetName());
		return Super::GetRequestIdentifier_Implementation();
	}

	const FModioFilterParams& FilterParams = FilterParamsPtr->Underlying;
	return FilterParams.ToString();
}

void UModioCommonFilteredModListView::NativePreConstruct()
{
	Super::NativePreConstruct();
#if WITH_EDITOR
	if (IsDesignTime())
	{
		CurrentPageIndex = PreviewCurrentPageIndex;
		TotalMods = PreviewTotalMods;
		PageSize = PreviewPageSize;
		SetInitialScreenVisibility(bPreviewShowInitialScreen);
		SetNoResultsVisibility(bPreviewShowNoResults);
		SetPageNavigationVisibility(bPreviewShowPageNavigation);
		SynchronizeProperties();
	}
#endif
}

void UModioCommonFilteredModListView::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// Set a default object for the datasource
	DataSource = NewObject<UModioFilterParamsUI>();

	SetInitialScreenVisibility(true);
	SetNoResultsVisibility(false);
	IModioUIAsyncOperationWidget::Execute_NotifyOperationState(this, EModioUIAsyncOperationWidgetState::InProgress);
	SetPageNavigationVisibility(false);
	IModioUIModInfoReceiver::Register<UModioCommonFilteredModListView>(EModioUIModInfoEventType::ListAllMods);
	IModioUIUserChangedReceiver::Register<UModioCommonFilteredModListView>();

	if (ErrorWithRetryWidget)
	{
		ErrorWithRetryWidget->RetryClickedDelegate.AddWeakLambda(this, [this]() {
			UModioFilterParamsUI* FilterParamsPtr = Cast<UModioFilterParamsUI>(DataSource);
			if (!FilterParamsPtr)
			{
				UE_LOG(ModioUI, Error, TEXT("Unable to request filtered mod list in '%s': Filter params are invalid"), *GetName());
				return;
			}
			SetDataSource(FilterParamsPtr);
		});
	}

	if (ModListLoader && ModListLoader->Implements<UModioUIAsyncHandlerWidget>())
	{
		TScriptInterface<IModioUIAsyncOperationWidget> InterfaceWrapper = TScriptInterface<IModioUIAsyncOperationWidget>(this);
		IModioUIAsyncHandlerWidget::Execute_LinkAsyncOperationWidget(ModListLoader, InterfaceWrapper);
	}
}

void UModioCommonFilteredModListView::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	
	TotalPages = PageSize > 0 ? (TotalMods + PageSize - 1) / PageSize : 0;
	
	if (const UModioCommonUISettings* UISettings = GetDefault<UModioCommonUISettings>())
	{
		if (PreviousPageButton)
		{
			PreviousPageButton->SetLabel(UISettings->FilteredModListParams.PreviousPageLabel);
		}

		if (NextPageButton)
		{
			NextPageButton->SetLabel(UISettings->FilteredModListParams.NextPageLabel);
		}

		if (CurrentPageTextBlock)
		{
			CurrentPageTextBlock->SetText(FText::Format(UISettings->FilteredModListParams.CurrentPageTextFormat, FText::AsNumber(CurrentPageIndex + 1)));
		}

		if (TotalPagesTextBlock)
		{
			TotalPagesTextBlock->SetText(FText::Format(UISettings->FilteredModListParams.TotalPagesTextFormat, FText::AsNumber(TotalPages)));
		}

		if (TotalModsTextBlock)
		{
			TotalModsTextBlock->SetText(FText::Format(TotalMods == 1 ? UISettings->FilteredModListParams.TotalSingleModTextFormat : UISettings->FilteredModListParams.TotalModsTextFormat, FText::AsNumber(TotalMods)));
		}
	}
	else
	{
		UE_LOG(ModioUI, Error, TEXT("Unable to synchronize some properties for '%s': Settings are invalid"), *GetName());
	}
	
	if (const UModioCommonFilteredModListViewStyle* StyleCDO = ModioStyle.GetDefaultObject())
	{
		if (UModioCommonListView* ModsListView = Cast<UModioCommonListView>(ModList))
		{
			ModsListView->SetStyle(StyleCDO->ModListStyle);
		}
		else if (UModioCommonModTileView* ModsTileView = Cast<UModioCommonModTileView>(ModList))
		{
			ModsTileView->SetStyle(StyleCDO->ModListStyle);
		}
		
		if (PreviousPageButton)
		{
			PreviousPageButton->SetStyle(StyleCDO->PreviousPageButtonStyle);
		}

		if (NextPageButton)
		{
			NextPageButton->SetStyle(StyleCDO->NextPageButtonStyle);
		}

		if (CurrentPageTextBlock)
		{
			CurrentPageTextBlock->SetStyle(StyleCDO->CurrentPageTextBlockStyle);
		}

		if (TotalPagesTextBlock)
		{
			TotalPagesTextBlock->SetStyle(StyleCDO->TotalPagesTextBlockStyle);
		}

		if (TotalModsTextBlock)
		{
			TotalModsTextBlock->SetStyle(StyleCDO->TotalModsTextBlockStyle);
		}
	}

	UpdateInputActions();
}

void UModioCommonFilteredModListView::NativeUserChanged(TOptional<FModioUser> NewUser)
{
	IModioUIUserChangedReceiver::NativeUserChanged(NewUser);
	UModioFilterParamsUI* FilterParamsPtr = Cast<UModioFilterParamsUI>(DataSource);
	if (!FilterParamsPtr)
	{
		UE_LOG(ModioUI, Error, TEXT("Unable to request filtered mod list in '%s': Filter params are invalid"), *GetName());
		return;
	}
	SetDataSource(FilterParamsPtr);
}

void UModioCommonFilteredModListView::SetModSelectionByID_Implementation(FModioModID ModID)
{
	IModioCommonModListViewInterface::SetModSelectionByID_Implementation(ModID);
	if (ModList && ModList->Implements<UModioCommonModListViewInterface>())
	{
		IModioCommonModListViewInterface::Execute_SetModSelectionByID(ModList, ModID);
	}
	else
	{
		UE_LOG(ModioUI, Error, TEXT("Unable to set selection for ModsView in '%s': ModsView either not bound or does not implement IModioCommonModListViewInterface"), *GetName());
	}
}

void UModioCommonFilteredModListView::SetFocusOnceListIsPopulated_Implementation(bool bFocus)
{
	IModioCommonModListViewInterface::SetFocusOnceListIsPopulated_Implementation(bFocus);
	if (ModList && ModList->Implements<UModioCommonModListViewInterface>())
	{
		IModioCommonModListViewInterface::Execute_SetFocusOnceListIsPopulated(ModList, bFocus);
	}
	else
	{
		UE_LOG(ModioUI, Error, TEXT("Unable to set focus for ModsView in '%s': ModsView either not bound or does not implement IModioCommonModListViewInterface"), *GetName());
	}
}

void UModioCommonFilteredModListView::RequestFullClearSelection_Implementation(bool bResetPreviouslySelected)
{
	IModioCommonModListViewInterface::RequestFullClearSelection_Implementation(bResetPreviouslySelected);
	if (ModList && ModList->Implements<UModioCommonModListViewInterface>())
	{
		IModioCommonModListViewInterface::Execute_RequestFullClearSelection(ModList, bResetPreviouslySelected);
	}
	else
	{
		UE_LOG(ModioUI, Error, TEXT("Unable to clear selection for ModsView in '%s': ModsView either not bound or does not implement IModioCommonModListViewInterface"), *GetName());
	}
}

void UModioCommonFilteredModListView::NativeSetListItems(const TArray<UObject*>& InListItems, bool bAddToExisting)
{
	IModioCommonModListViewInterface::NativeSetListItems(InListItems, bAddToExisting);
	if (ModList && ModList->Implements<UModioCommonModListViewInterface>())
	{
		Cast<IModioCommonModListViewInterface>(ModList)->NativeSetListItems(InListItems, bAddToExisting);
		OnSetModsFromModInfoListFinished.Broadcast();
		OnSetModsFromModInfoListDynamicFinished.Broadcast();
	}
	else
	{
		UE_LOG(ModioUI, Error, TEXT("Unable to set list items for ModsView in '%s': ModsView either not bound or does not implement IModioCommonModListViewInterface"), *GetName());
	}
}

void UModioCommonFilteredModListView::NativeOnListAllModsRequestCompleted(FString RequestIdentifier, FModioErrorCode ErrorCode, TOptional<FModioModInfoList> List)
{
	IModioUIModInfoReceiver::NativeOnListAllModsRequestCompleted(RequestIdentifier, ErrorCode, List);

	if (RequestIdentifier != GetRequestIdentifier())
	{
		return;
	}

	UModioFilterParamsUI* FilterParamsPtr = Cast<UModioFilterParamsUI>(DataSource);
	if (!FilterParamsPtr)
	{
		SetInitialScreenVisibility(false);
		SetNoResultsVisibility(true);
		if (ErrorWithRetryWidget)
		{
			if (UModioCommonErrorWithRetryParamsUI* ErrorWithRetryParams = NewObject<UModioCommonErrorWithRetryParamsUI>())
			{
				ErrorWithRetryParams->ErrorDescriptionTextLabel = NSLOCTEXT("Modio", "InvalidFilterParams", "Filter params are invalid");
				ErrorWithRetryWidget->SetDataSource(ErrorWithRetryParams);
			}
		}
		IModioUIAsyncOperationWidget::Execute_NotifyOperationState(this, EModioUIAsyncOperationWidgetState::Error);
		FocusOnDesiredWidget();
		UE_LOG(ModioUI, Error, TEXT("Unable to get requested filtered mod list in '%s': Filter params are invalid"),
			   *GetName());
		return;
	}
	
	if (ErrorCode)
	{
		SetInitialScreenVisibility(false);
		SetNoResultsVisibility(true);
		if (ErrorWithRetryWidget)
		{
			if (UModioCommonErrorWithRetryParamsUI* ErrorWithRetryParams = NewObject<UModioCommonErrorWithRetryParamsUI>())
			{
				ErrorWithRetryParams->ErrorDescriptionTextLabel = FText::FromString(ErrorCode.GetErrorMessage());
				ErrorWithRetryWidget->SetDataSource(ErrorWithRetryParams);
			}
		}
		IModioUIAsyncOperationWidget::Execute_NotifyOperationState(this, EModioUIAsyncOperationWidgetState::Error);
		FocusOnDesiredWidget();
		UE_LOG(ModioUI, Error, TEXT("Unable to get requested filtered mod list in '%s': %s"), *GetName(), *ErrorCode.GetErrorMessage());
		return;
	}

	UE_LOG(ModioUI, Log, TEXT("Successfully requested filtered mod list in '%s'.\nRequest identifier: %s, List size: %d"), *GetName(), *RequestIdentifier, List->GetRawList().Num());

	const FModioModInfoList& ModInfoList = List.GetValue();

	CurrentPageIndex = ModInfoList.GetPageIndex();
	TotalMods = ModInfoList.GetTotalResultCount();
	PageSize = ModInfoList.GetPageSize();

	SynchronizeProperties();

	if (Implements<UModioCommonModListViewInterface>())
	{
		Execute_SetModsFromModInfoList(this, List.GetValue(), false);
		if (ModInfoList.GetRawList().Num() > 0)
		{
			Execute_SetModSelectionByID(this, List->GetRawList()[0].ModId);
		}
	}

	if (TotalMods > 0)
	{
		SetPageNavigationVisibility(true);
		SetInitialScreenVisibility(false);
		SetNoResultsVisibility(false);
	}
	else
	{
		if (bHasSearchedBefore)
		{
			SetInitialScreenVisibility(false);
			SetNoResultsVisibility(true);
		}
		else
		{
			SetInitialScreenVisibility(true);
			SetNoResultsVisibility(false);
		}
	}

	IModioUIAsyncOperationWidget::Execute_NotifyOperationState(this, EModioUIAsyncOperationWidgetState::Success);

	// We should delay the focus on the desired widget to the next frame as the list view will not be fully refreshed until then
	FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateWeakLambda(this, [this](float DeltaTime)
	{
		if (TotalMods > 0 && GetNumItems() <= 0)
		{
			// If we have mods but no items, we need to wait for the list to be populated
			return true;
		}

		FocusOnDesiredWidget();
		// We only want to focus once, so we remove the ticker
		return false;
	}));
}

void UModioCommonFilteredModListView::SetInitialScreenVisibility_Implementation(bool bIsVisible)
{
	if (InitialScreenContainer)
	{
		InitialScreenContainer->SetVisibility(bIsVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
}

void UModioCommonFilteredModListView::SetNoResultsVisibility_Implementation(bool bIsVisible)
{
	if (NoResultsContainer)
	{
		NoResultsContainer->SetVisibility(bIsVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
}

void UModioCommonFilteredModListView::UpdateInputActions_Implementation()
{
	if (const UModioCommonUISettings* UISettings = GetDefault<UModioCommonUISettings>())
	{
		ClearListeningInputActions();

		if (PreviousPageButton && CurrentPageIndex > 0)
		{
			PreviousPageButton->SetVisibility(ESlateVisibility::Visible);
			ListenForInputAction(PreviousPageButton, UISettings->FilteredModListParams.PreviousPageInputAction, UISettings->FilteredModListParams.PreviousPageLabel, FOnModioCommonActivatableWidgetActionFiredFast::CreateWeakLambda(this, [this]() {
				HandlePreviousPageClicked();
			}));
		}
		else
		{
			if (PreviousPageButton) PreviousPageButton->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (NextPageButton && CurrentPageIndex < TotalPages - 1 && TotalPages > 1)
		{
			NextPageButton->SetVisibility(ESlateVisibility::Visible);
			ListenForInputAction(NextPageButton, UISettings->FilteredModListParams.NextPageInputAction, UISettings->FilteredModListParams.NextPageLabel, FOnModioCommonActivatableWidgetActionFiredFast::CreateWeakLambda(this, [this]() {
				HandleNextPageClicked();
			}));
		}
		else
		{
			if (NextPageButton) NextPageButton->SetVisibility(ESlateVisibility::Collapsed);
		}

		UnbindInputActions();
		BindInputActions();
	}
	else
	{
		UE_LOG(ModioUI, Error, TEXT("Unable to update input actions for '%s': Settings are invalid"), *GetName());
	}
}

void UModioCommonFilteredModListView::SetPageNavigationVisibility_Implementation(bool bIsVisible)
{
	if (PageNavigationContainer)
	{
		PageNavigationContainer->SetVisibility(bIsVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
}

void UModioCommonFilteredModListView::HandlePreviousPageClicked_Implementation()
{
	UModioFilterParamsUI* FilterParamsPtr = Cast<UModioFilterParamsUI>(DataSource);
	if (!FilterParamsPtr)
	{
		UE_LOG(ModioUI, Error, TEXT("Unable to go to previous page in '%s': Filter params are invalid"), *GetName());
		return;
	}

	FModioFilterParams& FilterParams = FilterParamsPtr->Underlying;
	FilterParams.PagedResults(CurrentPageIndex - 1, FilterParams.Count);
	SetDataSource(FilterParamsPtr);
}

void UModioCommonFilteredModListView::HandleNextPageClicked_Implementation()
{
	UModioFilterParamsUI* FilterParamsPtr = Cast<UModioFilterParamsUI>(DataSource);
	if (!FilterParamsPtr)
	{
		UE_LOG(ModioUI, Error, TEXT("Unable to go to next page in '%s': Filter params are invalid"), *GetName());
		return;
	}

	FModioFilterParams& FilterParams = FilterParamsPtr->Underlying;
	FilterParams.PagedResults(CurrentPageIndex + 1, FilterParams.Count);
	SetDataSource(FilterParamsPtr);
}
