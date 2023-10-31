/*
*  Copyright (C) 2021-2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */


#pragma once

#include "Misc/DateTime.h"
#include "Types/ModioCommonTypes.h"
#include "Types/ModioHeaderImage.h"

#include "ModioGamePlatform.generated.h"

/**
 * Details about a platform that a Game supports
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioGamePlatform
{
	GENERATED_BODY()

	/** @brief A platform supported by a title */
	UPROPERTY(BlueprintReadOnly, Category="mod.io|GamePlatform")
	EModioModfilePlatform Platform;

	/** @brief Whether ot not this platform is locked from having files submitted to it by players */
	UPROPERTY(BlueprintReadOnly, Category="mod.io|GamePlatform")
	bool Locked;

	/** @brief Whether or not this platform's file submissions are moderated or not */
	UPROPERTY(BlueprintReadOnly, Category="mod.io|GamePlatform")
	bool Moderated;
};