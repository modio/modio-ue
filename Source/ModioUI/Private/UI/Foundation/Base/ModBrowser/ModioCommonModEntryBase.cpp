/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Foundation/Base/ModBrowser/ModioCommonModEntryBase.h"

#include "Components/ListView.h"
#include "Components/ListViewBase.h"
#include "Core/ModioModCollectionEntryUI.h"
#include "Core/ModioModInfoUI.h"
#include "Misc/EngineVersionComparison.h"
#include "ModioUI.h"
#include "UI/Foundation/Components/List/ModioCommonModListViewInterface.h"
#include "UI/Foundation/Utilities/ModOperationTracker/ModioCommonModOperationTrackerUserWidget.h"
#include "UI/Foundation/Utilities/ModOperationTracker/ModioCommonModOperationTrackerWidget.h"

UModioCommonModEntryBase::UModioCommonModEntryBase()
{
	bAutoActivate = true;
#if UE_VERSION_OLDER_THAN(5, 2, 0)
	bIsFocusable = false;
#else
	SetIsFocusable(false);
#endif
	bAutoFocusOnActivation = false;
	bSupportsActivationFocus = false;
	bAutoBindInputAction = false;
}

bool UModioCommonModEntryBase::IsModListItemValid_Implementation() const
{
	return bEntrySet && IsValid(GetOwningListView()) && Cast<UListView>(GetOwningListView())
	       && !Cast<UListView>(GetOwningListView())->IsRefreshPending();
}

bool UModioCommonModEntryBase::IsModListItemSelected_Implementation() const
{
	if (CachedSelectionState.IsSet())
	{
		return CachedSelectionState.GetValue();
	}
	if (IsModListItemValid())
	{
		return IsListItemSelected();
	}
	return false;
}

void UModioCommonModEntryBase::SelectModListItem_Implementation()
{
	if (IsModListItemValid())
	{
		if (UListView* ListView = Cast<UListView>(GetOwningListView()))
		{
			ListView->SetSelectedItem(DataSource);
		}
	}
}

void UModioCommonModEntryBase::DeselectModListItem_Implementation()
{
	if (IsModListItemValid() && IsModListItemSelected())
	{
		if (UListView* ListView = Cast<UListView>(GetOwningListView()))
		{
			if (ListView->Implements<UModioCommonModListViewInterface>())
			{
				IModioCommonModListViewInterface::Execute_RequestFullClearSelection(ListView, false);
			}
			else
			{
				ListView->ClearSelection();
			}
		}
	}
}

FModioModID UModioCommonModEntryBase::GetModID_Implementation()
{
	if (!DataSource)
	{
		UE_LOG(ModioUI, Error, TEXT("Unable to get mod ID for mod entry '%s': empty DataSource"), *GetName());
		return FModioModID();
	}

	if (!DataSource->Implements<UModioModInfoUIDetails>())
	{
		UE_LOG(
			ModioUI, Error,
			TEXT("Unable to get mod ID for mod entry '%s': DataSource '%s' does not implement UModioModInfoUIDetails"),
			*GetName(), *DataSource->GetName());
		return FModioModID();
	}

	return IModioModInfoUIDetails::Execute_GetModID(DataSource);
}

FModioModInfo UModioCommonModEntryBase::GetFullModInfo_Implementation()
{
	if (!DataSource)
	{
		UE_LOG(ModioUI, Error, TEXT("Unable to get mod info for mod entry '%s': empty DataSource"), *GetName());
		return FModioModInfo();
	}

	if (!DataSource->Implements<UModioModInfoUIDetails>())
	{
		UE_LOG(
			ModioUI, Error,
			TEXT(
				"Unable to get mod info for mod entry '%s': DataSource '%s' does not implement UModioModInfoUIDetails"),
			*GetName(), *DataSource->GetName());
		return FModioModInfo();
	}

	return Execute_GetFullModInfo(DataSource);
}

bool UModioCommonModEntryBase::IsModSubscribed_Implementation() const
{
	if (ModOperationTrackerUserWidget && ModOperationTrackerUserWidget->Implements<UModioExtendedModInfoUIDetails>())
		return Execute_IsModSubscribed(ModOperationTrackerUserWidget);
	if (ModOperationTrackerWidget && ModOperationTrackerWidget->Implements<UModioExtendedModInfoUIDetails>())
		return Execute_IsModSubscribed(ModOperationTrackerWidget);
	UE_LOG(ModioUI, Verbose,
		   TEXT("Unable to determine if mod is subscribed for '%s': neither ModOperationTrackerUserWidget nor "
				"ModOperationTrackerWidget are bound"),
		   *GetName());
	return false;
}

bool UModioCommonModEntryBase::IsModDownloading_Implementation() const
{
	if (ModOperationTrackerUserWidget && ModOperationTrackerUserWidget->Implements<UModioExtendedModInfoUIDetails>())
		return Execute_IsModDownloading(ModOperationTrackerUserWidget);
	if (ModOperationTrackerWidget && ModOperationTrackerWidget->Implements<UModioExtendedModInfoUIDetails>())
		return Execute_IsModDownloading(ModOperationTrackerWidget);
	UE_LOG(ModioUI, Verbose,
		   TEXT("Unable to determine if mod is downloading for '%s': neither ModOperationTrackerUserWidget nor "
				"ModOperationTrackerWidget are bound"),
		   *GetName());
	return false;
}

bool UModioCommonModEntryBase::IsModExtracting_Implementation() const
{
	if (ModOperationTrackerUserWidget && ModOperationTrackerUserWidget->Implements<UModioExtendedModInfoUIDetails>())
		return Execute_IsModExtracting(ModOperationTrackerUserWidget);
	if (ModOperationTrackerWidget && ModOperationTrackerWidget->Implements<UModioExtendedModInfoUIDetails>())
		return Execute_IsModExtracting(ModOperationTrackerWidget);
	UE_LOG(ModioUI, Verbose,
		   TEXT("Unable to determine if mod is extracting for '%s': neither ModOperationTrackerUserWidget nor "
				"ModOperationTrackerWidget are bound"),
		   *GetName());
	return false;
}

bool UModioCommonModEntryBase::IsModInQueue_Implementation() const
{
	if (ModOperationTrackerUserWidget && ModOperationTrackerUserWidget->Implements<UModioExtendedModInfoUIDetails>())
		return Execute_IsModInQueue(ModOperationTrackerUserWidget);
	if (ModOperationTrackerWidget && ModOperationTrackerWidget->Implements<UModioExtendedModInfoUIDetails>())
		return Execute_IsModInQueue(ModOperationTrackerWidget);
	UE_LOG(ModioUI, Verbose,
		   TEXT("Unable to determine if mod is in queue for '%s': neither ModOperationTrackerUserWidget nor "
				"ModOperationTrackerWidget are bound"),
		   *GetName());
	return false;
}

bool UModioCommonModEntryBase::IsModEnabled_Implementation()
{
	if (ModOperationTrackerUserWidget && ModOperationTrackerUserWidget->Implements<UModioExtendedModInfoUIDetails>())
		return Execute_IsModEnabled(ModOperationTrackerUserWidget);
	if (ModOperationTrackerWidget && ModOperationTrackerWidget->Implements<UModioExtendedModInfoUIDetails>())
		return Execute_IsModEnabled(ModOperationTrackerWidget);
	UE_LOG(ModioUI, Verbose,
		   TEXT("Unable to determine if mod is enabled for '%s': neither ModOperationTrackerUserWidget nor "
				"ModOperationTrackerWidget are bound"),
		   *GetName());
	return false;
}

bool UModioCommonModEntryBase::IsModInstalled_Implementation() const
{
	if (ModOperationTrackerUserWidget && ModOperationTrackerUserWidget->Implements<UModioExtendedModInfoUIDetails>())
		return Execute_IsModInstalled(ModOperationTrackerUserWidget);
	if (ModOperationTrackerWidget && ModOperationTrackerWidget->Implements<UModioExtendedModInfoUIDetails>())
		return Execute_IsModInstalled(ModOperationTrackerWidget);
	UE_LOG(ModioUI, Verbose,
		   TEXT("Unable to determine if mod is installed for '%s': neither ModOperationTrackerUserWidget nor "
				"ModOperationTrackerWidget are bound"),
		   *GetName());
	return false;
}

bool UModioCommonModEntryBase::HasModErrors_Implementation() const
{
	if (ModOperationTrackerUserWidget && ModOperationTrackerUserWidget->Implements<UModioExtendedModInfoUIDetails>())
		return Execute_HasModErrors(ModOperationTrackerUserWidget);
	if (ModOperationTrackerWidget && ModOperationTrackerWidget->Implements<UModioExtendedModInfoUIDetails>())
		return Execute_HasModErrors(ModOperationTrackerWidget);
	UE_LOG(ModioUI, Verbose,
		   TEXT("Unable to determine if mod has errors for '%s': neither ModOperationTrackerUserWidget nor "
				"ModOperationTrackerWidget are bound"),
		   *GetName());
	return false;
}

void UModioCommonModEntryBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	IModioUIMediaDownloadCompletedReceiver::Register<UModioCommonModEntryBase>(EModioUIMediaDownloadEventType::ModLogo);
	IModioUIAuthenticationChangedReceiver::Register<UModioCommonModEntryBase>();
	IModioUIUserChangedReceiver::Register<UModioCommonModEntryBase>();
	IModioUISubscriptionsChangedReceiver::Register<UModioCommonModEntryBase>();
	IModioUIModManagementEventReceiver::Register<UModioCommonModEntryBase>();
	IModioUIModEnableWidget::Register<UModioCommonModEntryBase>();
}

void UModioCommonModEntryBase::NativeConstruct()
{
	Super::NativeConstruct();
	UnbindInputActions();
	NativeUpdateStyling(false);
}

void UModioCommonModEntryBase::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	NativeUpdateStyling(IsModListItemSelected());
}

void UModioCommonModEntryBase::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	bEntrySet = true;
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	SetDataSource(ListItemObject);
	NativeUpdateStyling(IsModListItemSelected());
}

void UModioCommonModEntryBase::NativeOnItemSelectionChanged(bool bIsSelected)
{
	IUserObjectListEntry::NativeOnItemSelectionChanged(bIsSelected);
	CachedSelectionState = bIsSelected;
	UpdateInputActions();
	NativeUpdateStyling(IsModListItemSelected());
	CachedSelectionState.Reset();
}

void UModioCommonModEntryBase::NativeOnEntryReleased()
{
	IUserObjectListEntry::NativeOnEntryReleased();
	bEntrySet = false;
	UnbindInputActions();
}

void UModioCommonModEntryBase::NativeOnSetDataSource()
{
	Super::NativeOnSetDataSource();

	const FModioModInfo ModInfo = Execute_GetFullModInfo(this);

	if (ModOperationTrackerUserWidget)
	{
		ModOperationTrackerUserWidget->SetTrackingModID(ModInfo.ModId);
	}

	if (ModOperationTrackerWidget)
	{
		ModOperationTrackerWidget->SetTrackingModID(ModInfo.ModId);
	}

	SynchronizeProperties();
}

void UModioCommonModEntryBase::NativeUserChanged(TOptional<FModioUser> NewUser)
{
	IModioUIUserChangedReceiver::NativeUserChanged(NewUser);
	bIsUserAuthenticated = NewUser.IsSet();
}

void UModioCommonModEntryBase::NativeOnModManagementEvent(FModioModManagementEvent Event)
{
	IModioUIModManagementEventReceiver::NativeOnModManagementEvent(Event);

	const FModioModID ModID = Execute_GetModID(this);

	if (DataSource && ModID == Event.ID)
	{
		SetDataSource(DataSource);
		UpdateInputActions();
	}
}

void UModioCommonModEntryBase::NativeOnSubscriptionsChanged(FModioModID ModID, bool bNewSubscriptionState)
{
	IModioUISubscriptionsChangedReceiver::NativeOnSubscriptionsChanged(ModID, bNewSubscriptionState);

	const FModioModID CurrentModID = Execute_GetModID(this);

	if (DataSource && CurrentModID == ModID)
	{
		SetDataSource(DataSource);
		UpdateInputActions();
	}
}

void UModioCommonModEntryBase::NativeOnModEnabledStateChanged(FModioModID ModID, bool bNewSubscriptionState)
{
	IModioUIModEnableWidget::NativeOnModEnabledStateChanged(ModID, bNewSubscriptionState);

	const FModioModID CurrentModID = Execute_GetModID(this);

	if (DataSource && CurrentModID == ModID)
	{
		SetDataSource(DataSource);
		UpdateInputActions();
	}
}

void UModioCommonModEntryBase::NativeUpdateStyling(bool bIsListItemSelected)
{
	UpdateStyling(bIsListItemSelected);
}

void UModioCommonModEntryBase::UpdateInputActions_Implementation()
{
	if (IsModListItemSelected())
	{
		UnbindInputActions();
		BindInputActions();
	}
	else
	{
		UnbindInputActions();
	}
}

bool UModioCommonModEntryBase::IsUserAuthenticated() const
{
	return bIsUserAuthenticated;
}
