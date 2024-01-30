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
#include "UI/Foundation/Base/ModioCommonActivatableWidget.h"
#include "UI/EventHandlers/IModioUIModManagementEventReceiver.h"
#include "UI/EventHandlers/IModioUISubscriptionsChangedReceiver.h"
#include "UI/EventHandlers/IModioUIUserChangedReceiver.h"
#include "UI/Interfaces/IModioExtendedModInfoUIDetails.h"
#include "UI/Interfaces/IModioUIModEnableWidget.h"

#include "ModioCommonModDetailsViewBase.generated.h"

class UModioCommonModOperationTrackerUserWidget;
class UModioCommonModOperationTrackerWidget;

/**
 * Mod Details Base view widget that displays the details of a mod
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonModDetailsViewBase
	: public UModioCommonActivatableWidget,
	  public IModioUIModEnableWidget,
	  public IModioUIUserChangedReceiver,
	  public IModioUIModManagementEventReceiver,
	  public IModioUISubscriptionsChangedReceiver,
	  public IModioModInfoUIDetails,
	  public IModioExtendedModInfoUIDetails
{
	GENERATED_BODY()

protected:
	/** The mod operation tracker user widget. Can be used to track and display the status of the current mod operation as a standalone widget */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets|Tracker")
	TObjectPtr<UModioCommonModOperationTrackerUserWidget> ModOperationTrackerUserWidget;

	/** The mod operation tracker widget. Can be used to track the status of the current mod operations */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets|Tracker")
	TObjectPtr<UModioCommonModOperationTrackerWidget> ModOperationTrackerWidget;

public:
	//~ Begin IModioExtendedModInfoUIDetails Interface
	virtual FModioModID GetModID_Implementation() override;
	virtual FModioModInfo GetFullModInfo_Implementation() override;
	virtual bool IsModSubscribed_Implementation() const override;
	virtual bool IsModDownloading_Implementation() const override;
	virtual bool IsModExtracting_Implementation() const override;
	virtual bool IsModInQueue_Implementation() const override;
	virtual bool IsModEnabled_Implementation() override;
	virtual bool IsModInstalled_Implementation() const override;
	virtual bool HasModErrors_Implementation() const override;
	//~ End IModioExtendedModInfoUIDetails Interface

	/**
	 * Returns true if the user is authenticated
	 * @return True if the user is authenticated, false otherwise
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Info")
	bool IsUserAuthenticated() const;
protected:
	//~ Begin IModioUIUserChangedReceiver Interface
	virtual void NativeUserChanged(TOptional<FModioUser> NewUser) override;
	//~ End IModioUIUserChangedReceiver Interface
private:
	/** Whether the user is authenticated or not */
	bool bIsUserAuthenticated = false;

protected:
	//~ Begin UModioCommonActivatableWidget Interface
	virtual void NativeOnSetDataSource() override;
	virtual void NativeOnInitialized() override;
	//~ End UModioCommonActivatableWidget Interface
};
