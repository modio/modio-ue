/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "UI/EventHandlers/IModioUIAuthenticationChangedReceiver.h"
#include "UI/EventHandlers/IModioUIMediaDownloadCompletedReceiver.h"
#include "UI/EventHandlers/IModioUISubscriptionsChangedReceiver.h"
#include "UI/EventHandlers/IModioUIUserChangedReceiver.h"
#include "UI/Foundation/Base/ModioCommonActivatableWidget.h"
#include "UI/Interfaces/IModioUIModEnableWidget.h"
#include "UI/EventHandlers/IModioUIModManagementEventReceiver.h"
#include "UI/Interfaces/IModioExtendedModInfoUIDetails.h"
#include "Misc/Optional.h"
#include "ModioCommonModEntryBase.generated.h"

class UModioCommonModOperationTrackerUserWidget;
class UModioCommonModOperationTrackerWidget;

/**
 * A base class for mod entries that are used in the mod lists
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonModEntryBase
	: public UModioCommonActivatableWidget,
	  public IUserObjectListEntry,
	  public IModioUIUserChangedReceiver,
	  public IModioUIMediaDownloadCompletedReceiver,
	  public IModioUIAuthenticationChangedReceiver,
	  public IModioUISubscriptionsChangedReceiver,
	  public IModioUIModEnableWidget,
	  public IModioUIModManagementEventReceiver,
	  public IModioModInfoUIDetails,
	  public IModioExtendedModInfoUIDetails
{
	GENERATED_BODY()

public:
	UModioCommonModEntryBase();

protected:
	/** The mod operation tracker user widget. Can be used to track and display the status of the current mod operation as a standalone widget */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets|Tracker")
	TObjectPtr<UModioCommonModOperationTrackerUserWidget> ModOperationTrackerUserWidget;

	/** The mod operation tracker widget. Can be used to track the status of the current mod operations */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets|Tracker")
	TObjectPtr<UModioCommonModOperationTrackerWidget> ModOperationTrackerWidget;

public:
	/**
	 * Returns true if the mod list item is valid
	 * @note Every time when you need to work with the mod list item, you should check if it is valid with this function
	 * @return True if the mod list item is valid, false otherwise
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Check")
	bool IsModListItemValid() const;

	/**
	 * Returns true if the mod list item is currently selected
	 * @return True if the mod list item is currently selected, false otherwise
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Check")
	bool IsModListItemSelected() const;

	/**
	 * Selects the mod list item
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Select")
	void SelectModListItem();

	/**
	 * Deselects the mod list item
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Select")
	void DeselectModListItem();

	//~ Begin IModioUIModInfoReceiver Interface
	virtual FModioModID GetModID_Implementation() override;
	virtual FModioModInfo GetFullModInfo_Implementation() override;
	//~ End IModioUIModInfoReceiver Interface

	//~ Begin IModioExtendedModInfoUIDetails Interface
	virtual bool IsModSubscribed_Implementation() const override;
	virtual bool IsModDownloading_Implementation() const override;
	virtual bool IsModExtracting_Implementation() const override;
	virtual bool IsModInQueue_Implementation() const override;
	virtual bool IsModEnabled_Implementation() override;
	virtual bool IsModInstalled_Implementation() const override;
	virtual bool HasModErrors_Implementation() const override;
	//~ End IModioExtendedModInfoUIDetails Interface

protected:
	//~ Begin UWidget Interface
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
public:
	virtual void SynchronizeProperties() override;
protected:
	//~ End UWidget Interface

	//~ Begin IUserObjectListEntry Interface
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;
	virtual void NativeOnEntryReleased() override;
	//~ End IUserObjectListEntry Interface

	//~ Begin UModioCommonActivatableWidget Interface
	virtual void NativeOnSetDataSource() override;
	//~ End UModioCommonActivatableWidget Interface

	//~ Begin IModioUIUserChangedReceiver Interface
	virtual void NativeUserChanged(TOptional<FModioUser> NewUser) override;
	//~ End IModioUIUserChangedReceiver Interface

	//~ Begin IModioUIModManagementEventReceiver Interface
	virtual void NativeOnModManagementEvent(FModioModManagementEvent Event) override;
	//~ End IModioUIModManagementEventReceiver Interface

	//~ Begin IModioUISubscriptionsChangedReceiver Interface
	virtual void NativeOnSubscriptionsChanged(FModioModID ModID, bool bNewSubscriptionState) override;
	//~ End IModioUISubscriptionsChangedReceiver Interface

	//~ Begin IModioUIModEnableWidget Interface
	virtual void NativeOnModEnabledStateChanged(FModioModID ModID, bool bNewSubscriptionState) override;
	//~ End IModioUIModEnableWidget Interface

	/**
	 * Updates the styling of the widget based on the current selection state
	 * @param bIsListItemSelected Whether the entry is currently selected
	 */
	virtual void NativeUpdateStyling(bool bIsListItemSelected);

	/**
	 * Updates the styling of the widget based on the current selection state
	 * @param bIsListItemSelected Whether the entry is currently selected
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Mod.io Common UI|Update")
	void UpdateStyling(bool bIsListItemSelected);

	/**
	 * Updates the input actions of the widget based on the current selection state
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Action")
	void UpdateInputActions();

	/**
	 * Returns true if the user is authenticated
	 * @return True if the user is authenticated, false otherwise
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Info")
	bool IsUserAuthenticated() const;

private:
	/** Whether the entry has been set up or not */
	bool bEntrySet = false;

	/** Whether the user is authenticated or not */
	bool bIsUserAuthenticated = false;

	/**
	 * The cached selection state
	 * The IUserListEntry::IsListItemSelected doesn't return the correct value when the IUserListEntry::NativeOnItemSelectionChanged is called too early (before the construction of entry is finished), so we need to cache the selection state
	 */
	TOptional<bool> CachedSelectionState;
};
