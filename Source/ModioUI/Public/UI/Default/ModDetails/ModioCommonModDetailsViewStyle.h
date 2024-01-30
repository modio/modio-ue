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
#include "ModioCommonModDetailsViewStyle.generated.h"

class UModioCommonModGalleryViewStyle;
class UModioCommonButtonStyle;
class UModioCommonTextStyle;
class UModioCommonProgressBarStyle;

/**
 * The style of the Mod Details view within the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonModDetailsViewStyle : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	TSubclassOf<UModioCommonModGalleryViewStyle> ModGalleryViewStyle;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	TSubclassOf<UModioCommonTextStyle> ModNameTextStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	TSubclassOf<UModioCommonButtonStyle> SubscribeButtonStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	TSubclassOf<UModioCommonButtonStyle> CancelButtonStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	TSubclassOf<UModioCommonButtonStyle> RateUpButtonStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	TSubclassOf<UModioCommonButtonStyle> RateDownButtonStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	TSubclassOf<UModioCommonButtonStyle> ReportButtonStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	TSubclassOf<UModioCommonTextStyle> ModSummaryTextStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	TSubclassOf<UModioCommonTextStyle> ModFullDescriptionLabelTextStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	TSubclassOf<UModioCommonTextStyle> ModFullDescriptionTextStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	TSubclassOf<UModioCommonTextStyle> DownloadProgressLabelTextStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	TSubclassOf<UModioCommonTextStyle> DownloadProgressTextStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	TSubclassOf<UModioCommonProgressBarStyle> DownloadProgressBarStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	TSubclassOf<UModioCommonTextStyle> StatsLabelTextStyle;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	TSubclassOf<UModioCommonTextStyle> StatsTextStyle;
};
