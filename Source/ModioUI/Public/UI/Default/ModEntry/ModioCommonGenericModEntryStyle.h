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
#include "ModioCommonGenericModEntryStyle.generated.h"

class UModioCommonModOperationTrackerUserWidgetStyle;
class UModioCommonBorderStyle;
class UModioCommonButtonStyle;
class UModioCommonDynamicImageStyle;
class UModioCommonTextStyle;
class UModioCommonRichTextStyle;
class UModioCommonProgressBarStyle;

/**
 * The style of the Generic Mod Entry within the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonGenericModEntryStateStyle : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style|Tracker")
	TSubclassOf<UModioCommonModOperationTrackerUserWidgetStyle> ModOperationTrackerStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style|Image")
	TSubclassOf<UModioCommonBorderStyle> BackgroundStyle;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style|Image")
	TSubclassOf<UModioCommonDynamicImageStyle> ModImageStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style|Text")
	TSubclassOf<UModioCommonRichTextStyle> ModNameTextStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style|Text|Operation")
	TSubclassOf<UModioCommonRichTextStyle> OperationLabelTextStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style|Text|Operation")
	TSubclassOf<UModioCommonRichTextStyle> OperationDetailsTextStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style|Text|Speed")
	TSubclassOf<UModioCommonRichTextStyle> SpeedLabelTextStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style|Text|Speed")
	TSubclassOf<UModioCommonRichTextStyle> SpeedDetailsTextStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style|Text|Downloads")
	TSubclassOf<UModioCommonRichTextStyle> DownloadsLabelTextStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style|Text|Downloads")
	TSubclassOf<UModioCommonRichTextStyle> DownloadsDetailsTextStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style|Text|Size")
	TSubclassOf<UModioCommonRichTextStyle> SizeLabelTextStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style|Text|Size")
	TSubclassOf<UModioCommonRichTextStyle> SizeDetailsTextStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style|Text|Rating")
	TSubclassOf<UModioCommonRichTextStyle> RatingPercentageTextStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style|Text|Rating")
	TSubclassOf<UModioCommonRichTextStyle> RatingPositiveTextStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style|Text|Rating")
	TSubclassOf<UModioCommonRichTextStyle> RatingNegativeTextStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style|Text|Progress")
	TSubclassOf<UModioCommonRichTextStyle> PercentProgressTextStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style|Button")
	TSubclassOf<UModioCommonButtonStyle> SubscribeButtonStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style|Button")
	TSubclassOf<UModioCommonButtonStyle> CancelButtonStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style|Button")
	TSubclassOf<UModioCommonButtonStyle> EnableButtonStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style|Button")
	TSubclassOf<UModioCommonButtonStyle> DisableButtonStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style|Button")
	TSubclassOf<UModioCommonButtonStyle> OpenModDetailsButtonStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style|Progress")
	TSubclassOf<UModioCommonProgressBarStyle> PercentProgressBarStyle;
};

UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonGenericModEntryStyle : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	TSubclassOf<UModioCommonGenericModEntryStateStyle> NormalStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	TSubclassOf<UModioCommonGenericModEntryStateStyle> SelectedStyle;
};
