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
#include "ModioCommonModEntryBase.generated.h"

class UModioCommonModOperationTrackerUserWidget;
class UModioCommonModOperationTrackerWidget;

/**
 * @brief A base class for mod entries that are used in the mod lists
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI5_API UModioCommonModEntryBase
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
	/** @brief The mod operation tracker user widget. Can be used to display the status of mod operations */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets|Tracker")
	TObjectPtr<UModioCommonModOperationTrackerUserWidget> ModOperationTrackerUserWidget;

	/** @brief The mod operation tracker widget. Can be used to display the status of mod operations */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets|Tracker")
	TObjectPtr<UModioCommonModOperationTrackerWidget> ModOperationTrackerWidget;

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Check")
	bool IsModListItemValid() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Check")
	bool IsModListItemSelected() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Select")
	void SelectModListItem();

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
	
	virtual void NativeUserChanged(TOptional<FModioUser> NewUser) override;
	virtual void NativeOnModManagementEvent(FModioModManagementEvent Event) override;
	virtual void NativeOnSubscriptionsChanged(FModioModID ModID, bool bNewSubscriptionState) override;
	virtual void NativeOnModEnabledStateChanged(FModioModID ModID, bool bNewSubscriptionState) override;

	/**
	 * @brief Updates the styling of the widget based on the current selection state
	 * @param bIsListItemSelected Whether the entry is currently selected
	 */
	virtual void NativeUpdateStyling(bool bIsListItemSelected);

	/**
	 * @brief Updates the styling of the widget based on the current selection state
	 * @param bIsListItemSelected Whether the entry is currently selected
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Mod.io Common UI|Update")
	void UpdateStyling(bool bIsListItemSelected);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Action")
	void UpdateInputActions();

	/**
	 * @brief Checks whether the user is authenticated or not
	 * @return True if the user is authenticated, false otherwise
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Info")
	bool IsUserAuthenticated() const;

private:
	bool bEntrySet = false;
	bool bIsUserAuthenticated = false;
};
