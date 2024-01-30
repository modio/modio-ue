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
#include "UObject/Object.h"
#include "Templates/SubclassOf.h"
#include "ModioCommonModOperationTrackerUserWidgetStyle.generated.h"

class UModioCommonImageStyle;
class UModioCommonTextStyle;
class UModioCommonProgressBarStyle;

/**
 * The style of the Mod Operation Tracker User Widget within the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonModOperationTrackerUserWidgetStyle : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	TSubclassOf<UModioCommonImageStyle> IconImageStyle;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	TSubclassOf<UModioCommonTextStyle> OverallOperationPercentageLabelTextStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	TSubclassOf<UModioCommonTextStyle> OverallOperationPercentageTextStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	TSubclassOf<UModioCommonTextStyle> QueuedOperationNumberLabelTextStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	TSubclassOf<UModioCommonTextStyle> QueuedOperationNumberTextStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	TSubclassOf<UModioCommonTextStyle> SpeedLabelTextStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	TSubclassOf<UModioCommonTextStyle> SpeedTextStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	TSubclassOf<UModioCommonProgressBarStyle> ModOperationProgressBarStyle;
};
