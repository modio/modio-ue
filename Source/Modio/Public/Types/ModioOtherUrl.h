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

#include "Containers/UnrealString.h"

#include "ModioOtherUrl.generated.h"

/**
 * Strong type struct representing creator defined urls to share
 **/
USTRUCT(BlueprintType)
struct MODIO_API FModioOtherUrl
{
	GENERATED_BODY()

	/** @brief Label of the link you are sharing */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|OtherUrl")
	FString Label;

	/** @brief The URL to be associated with the label */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|OtherUrl")
	FString Url;
};