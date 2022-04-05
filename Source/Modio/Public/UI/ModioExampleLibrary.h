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

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ModioSubsystem.h"
#include "Types/ModioCommonTypes.h"
#include "Types/ModioErrorCode.h"
#include "Types/ModioModInfoList.h"

// clang-format off
#include "ModioExampleLibrary.generated.h"
// clang-format on

UCLASS(MinimalAPI)
class UModioExampleLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/** Get the project specified avatar thumbnail size */
	UFUNCTION(BlueprintPure, Category = "mod.io|Example|Project")
	static EModioAvatarSize GetAvatarThumbnailSize();

	/** Get the project specified logo thumbnail size */
	UFUNCTION(BlueprintPure, Category = "mod.io|Example|Project")
	static EModioLogoSize GetLogoThumbnailSize();

	/** Get the project specified logo full size */
	UFUNCTION(BlueprintPure, Category = "mod.io|Example|Project")
	static EModioLogoSize GetLogoFullSize();

	/**
	 * @brief Runs a filter over the user's subscription list
	 * @param FilterParams Supports the following operations:
	 * @param Callback Called when mod list has been processed
	 **/
	UFUNCTION(BlueprintCallable, Category = "mod.io|Example|UserSubscription")
	static void ListUserSubscriptionAsync(const struct FModioFilterParams& FilterParams,
										  FOnListAllModsDelegate Callback);

	
};
