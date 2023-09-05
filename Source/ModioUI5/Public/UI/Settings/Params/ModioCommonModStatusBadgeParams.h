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
#include "Engine/DeveloperSettings.h"
#include "UI/Settings/ModioCommonDefines.h"
#include "ModioCommonModStatusBadgeParams.generated.h"

UCLASS(Config = "ModioCommonModStatusBadgeParams", DefaultConfig, meta = (DisplayName = "Mod Status Badge Params"))
class MODIOUI5_API UModioCommonSubscriptionBadgeParamsSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditDefaultsOnly, Category = "Mod.io Common UI|Widgets")
	FText PendingLabelText = NSLOCTEXT("Modio", "Pending", "Pending");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Mod.io Common UI|Widgets")
	FText DownloadingLabelText = NSLOCTEXT("Modio", "Downloading", "Downloading");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Mod.io Common UI|Widgets")
	FText ExtractingLabelText = NSLOCTEXT("Modio", "Extracting", "Extracting");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Mod.io Common UI|Widgets")
	FText SubscribedLabelText = NSLOCTEXT("Modio", "Subscribed", "Subscribed");

	// Begin UDeveloperSettings Interface
	virtual FName GetCategoryName() const override { return ModioCommonCategoryName; }
	// End UDeveloperSettings Interface
};
