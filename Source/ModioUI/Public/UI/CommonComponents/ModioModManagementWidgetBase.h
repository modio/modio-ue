/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#pragma once

#include "CoreMinimal.h"
#include "Types/ModioModProgressInfo.h"
#include "UI/BaseWidgets/ModioUserWidgetBase.h"
#include "UI/EventHandlers/IModioUIModManagementEventReceiver.h"
#include "UI/EventHandlers/IModioUISubscriptionsChangedReceiver.h"

#include "ModioModManagementWidgetBase.generated.h"

/**
 *
 */
UCLASS()
class MODIOUI_API UModioModManagementWidgetBase : public UModioUserWidgetBase,
												  public IModioUIModManagementEventReceiver,
												  public IModioUISubscriptionsChangedReceiver
{
	GENERATED_BODY()
protected:
	virtual void NativeOnInitialized() override;

	virtual void NativeOnSubscriptionsChanged(FModioModID ModID, bool bNewSubscriptionState) override;

	bool bShouldTick = false;

	virtual void SynchronizeProperties() override;
	void BeginTickIfNeeded(bool bShouldStartTicking);
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	bool UpdateProgressIfNeeded(bool bForceUpdate = false);

	virtual void UpdateProgress(const struct FModioModProgressInfo& ProgressInfo);

#if WITH_EDITORONLY_DATA
	UPROPERTY(Transient)
	FModioModProgressInfo PreviewProgressInfo;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Transient, meta = (ClampMin = 0), Category = "Widgets")
	int64 PreviewTotal = 1;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Transient, meta = (ClampMin = 0), Category = "Widgets")
	int64 PreviewCurrent = 0;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Transient, meta = (ClampMin = 0), Category = "Widgets")
	EModioModProgressState PreviewProgressState = EModioModProgressState::Initializing;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Transient, Category = "Widgets")
	FModioModID PreviewModID;

#endif
};
