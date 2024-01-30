/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Foundation/Base/ModDetails/ModioCommonModDetailsViewBase.h"
#include "ModioUI.h"
#include "UI/Foundation/Utilities/ModOperationTracker/ModioCommonModOperationTrackerUserWidget.h"
#include "UI/Foundation/Utilities/ModOperationTracker/ModioCommonModOperationTrackerWidget.h"

FModioModID UModioCommonModDetailsViewBase::GetModID_Implementation()
{
	if (!DataSource)
	{
		UE_LOG(ModioUI, Error, TEXT("Unable to get mod ID for mod details '%s': DataSource is invalid"), *GetName());
		return FModioModID();
	}
	
	if (!DataSource->Implements<UModioModInfoUIDetails>())
	{
		UE_LOG(ModioUI, Error, TEXT("Unable to get mod ID for mod details '%s': DataSource '%s' does not implement UModioModInfoUIDetails"), *GetName(), *DataSource->GetName());
		return FModioModID();
	}

	return IModioModInfoUIDetails::Execute_GetModID(DataSource);
}

FModioModInfo UModioCommonModDetailsViewBase::GetFullModInfo_Implementation()
{
	if (!DataSource)
	{
		UE_LOG(ModioUI, Error, TEXT("Unable to get mod info for mod details '%s': DataSource is invalid"), *GetName());
		return FModioModInfo();
	}
	
	if (!DataSource->Implements<UModioModInfoUIDetails>())
	{
		UE_LOG(ModioUI, Error, TEXT("Unable to get mod info for mod details '%s': DataSource '%s' does not implement UModioModInfoUIDetails"), *GetName(), *DataSource->GetName());
		return FModioModInfo();
	}

	return Execute_GetFullModInfo(DataSource);
}

bool UModioCommonModDetailsViewBase::IsModSubscribed_Implementation() const
{
	if (ModOperationTrackerUserWidget && ModOperationTrackerUserWidget->Implements<UModioExtendedModInfoUIDetails>()) return Execute_IsModSubscribed(ModOperationTrackerUserWidget);
	if (ModOperationTrackerWidget && ModOperationTrackerWidget->Implements<UModioExtendedModInfoUIDetails>()) return Execute_IsModSubscribed(ModOperationTrackerWidget);
	UE_LOG(ModioUI, Verbose, TEXT("Unable to determine if mod is subscribed for '%s': neither ModOperationTrackerUserWidget nor ModOperationTrackerWidget are bound"), *GetName());
	return false;
}

bool UModioCommonModDetailsViewBase::IsModDownloading_Implementation() const
{
	if (ModOperationTrackerUserWidget && ModOperationTrackerUserWidget->Implements<UModioExtendedModInfoUIDetails>()) return Execute_IsModDownloading(ModOperationTrackerUserWidget);
	if (ModOperationTrackerWidget && ModOperationTrackerWidget->Implements<UModioExtendedModInfoUIDetails>()) return Execute_IsModDownloading(ModOperationTrackerWidget);
	UE_LOG(ModioUI, Verbose, TEXT("Unable to determine if mod is downloading for '%s': neither ModOperationTrackerUserWidget nor ModOperationTrackerWidget are bound"), *GetName());
	return false;
}

bool UModioCommonModDetailsViewBase::IsModExtracting_Implementation() const
{
	if (ModOperationTrackerUserWidget && ModOperationTrackerUserWidget->Implements<UModioExtendedModInfoUIDetails>()) return Execute_IsModExtracting(ModOperationTrackerUserWidget);
	if (ModOperationTrackerWidget && ModOperationTrackerWidget->Implements<UModioExtendedModInfoUIDetails>()) return Execute_IsModExtracting(ModOperationTrackerWidget);
	UE_LOG(ModioUI, Verbose, TEXT("Unable to determine if mod is extracting for '%s': neither ModOperationTrackerUserWidget nor ModOperationTrackerWidget are bound"), *GetName());
	return false;
}

bool UModioCommonModDetailsViewBase::IsModInQueue_Implementation() const
{
	if (ModOperationTrackerUserWidget && ModOperationTrackerUserWidget->Implements<UModioExtendedModInfoUIDetails>()) return Execute_IsModInQueue(ModOperationTrackerUserWidget);
	if (ModOperationTrackerWidget && ModOperationTrackerWidget->Implements<UModioExtendedModInfoUIDetails>()) return Execute_IsModInQueue(ModOperationTrackerWidget);
	UE_LOG(ModioUI, Verbose, TEXT("Unable to determine if mod is in queue for '%s': neither ModOperationTrackerUserWidget nor ModOperationTrackerWidget are bound"), *GetName());
	return false;
}

bool UModioCommonModDetailsViewBase::IsModEnabled_Implementation()
{
	if (ModOperationTrackerUserWidget && ModOperationTrackerUserWidget->Implements<UModioExtendedModInfoUIDetails>()) return Execute_IsModEnabled(ModOperationTrackerUserWidget);
	if (ModOperationTrackerWidget && ModOperationTrackerWidget->Implements<UModioExtendedModInfoUIDetails>()) return Execute_IsModEnabled(ModOperationTrackerWidget);
	UE_LOG(ModioUI, Verbose, TEXT("Unable to determine if mod is enabled for '%s': neither ModOperationTrackerUserWidget nor ModOperationTrackerWidget are bound"), *GetName());
	return false;
}

bool UModioCommonModDetailsViewBase::IsModInstalled_Implementation() const
{
	if (ModOperationTrackerUserWidget && ModOperationTrackerUserWidget->Implements<UModioExtendedModInfoUIDetails>()) return Execute_IsModInstalled(ModOperationTrackerUserWidget);
	if (ModOperationTrackerWidget && ModOperationTrackerWidget->Implements<UModioExtendedModInfoUIDetails>()) return Execute_IsModInstalled(ModOperationTrackerWidget);
	UE_LOG(ModioUI, Verbose, TEXT("Unable to determine if mod is installed for '%s': neither ModOperationTrackerUserWidget nor ModOperationTrackerWidget are bound"), *GetName());
	return false;
}

bool UModioCommonModDetailsViewBase::HasModErrors_Implementation() const
{
	if (ModOperationTrackerUserWidget && ModOperationTrackerUserWidget->Implements<UModioExtendedModInfoUIDetails>()) return Execute_HasModErrors(ModOperationTrackerUserWidget);
	if (ModOperationTrackerWidget && ModOperationTrackerWidget->Implements<UModioExtendedModInfoUIDetails>()) return Execute_HasModErrors(ModOperationTrackerWidget);
	UE_LOG(ModioUI, Verbose, TEXT("Unable to determine if mod has errors for '%s': neither ModOperationTrackerUserWidget nor ModOperationTrackerWidget are bound"), *GetName());
	return false;
}

bool UModioCommonModDetailsViewBase::IsUserAuthenticated() const
{
	return bIsUserAuthenticated;
}

void UModioCommonModDetailsViewBase::NativeOnSetDataSource()
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
}

void UModioCommonModDetailsViewBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	IModioUIUserChangedReceiver::Register<UModioCommonModDetailsViewBase>();
	IModioUIModEnableWidget::Register<UModioCommonModDetailsViewBase>();
	IModioUISubscriptionsChangedReceiver::Register<UModioCommonModDetailsViewBase>();
	IModioUIModManagementEventReceiver::Register<UModioCommonModDetailsViewBase>();
}

void UModioCommonModDetailsViewBase::NativeUserChanged(TOptional<FModioUser> NewUser)
{
	IModioUIUserChangedReceiver::NativeUserChanged(NewUser);
	bIsUserAuthenticated = NewUser.IsSet();
}
