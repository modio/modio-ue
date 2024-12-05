/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE5 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#pragma once

#include "ModioOpenStoreResult.generated.h"

/**
 * @brief Enumerator of potential results on attempting to open a native platform store
 **/
UENUM(BlueprintType)
enum class EModioOpenStoreResult : uint8
{
	/** Store opened successfully **/
	Success,

	/** Monetization not active **/
	FailedInactive,

	/** The current platform does not support a native store **/
	FailedUnsupportedPlatform,

	/** Failed to open for an unknown reason **/
	FailedUnknown
};