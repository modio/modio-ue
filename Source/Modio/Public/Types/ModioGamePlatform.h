/*
*  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#pragma once

#include "Misc/DateTime.h"
#include "Types/ModioCommonTypes.h"
#include "Types/ModioHeaderImage.h"

#include "ModioGamePlatform.generated.h"

/**
 * @docpublic
 * @brief Details about a platform that a Game supports
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioGamePlatform
{
	GENERATED_BODY()

	/**
	 * @docpublic
	 * @brief A platform supported by a title
	 */
	UPROPERTY(BlueprintReadOnly, Category="mod.io|GamePlatform")
	EModioModfilePlatform Platform = EModioModfilePlatform::Windows;

	/**
	 * @docpublic
	 * @brief Whether or not this platform is locked from having files submitted to it by players
	 */
	UPROPERTY(BlueprintReadOnly, Category="mod.io|GamePlatform")
	bool Locked = false;

	/**
	 * @docpublic
	 * @brief Whether or not this platform's file submissions are moderated
	 */
	UPROPERTY(BlueprintReadOnly, Category="mod.io|GamePlatform")
	bool Moderated = false;
};