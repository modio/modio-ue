/*
 *  Copyright (C) 2025 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

 #pragma once

#include "ModioModMonetizationSKU.generated.h"


USTRUCT(BlueprintType)
struct MODIO_API FModioModMonetizationSKU
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "mod.io|Monetization")
    int64 Id;

    UPROPERTY(BlueprintReadOnly, Category = "mod.io|Monetization")
    FString SKUName;

    UPROPERTY(BlueprintReadOnly, Category = "mod.io|Monetization")
    FString Portal;

};