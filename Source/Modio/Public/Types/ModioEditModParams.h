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
#include "Types/ModioModInfo.h"
#include "Misc/Optional.h"

#include "ModioEditModParams.generated.h"

USTRUCT(BlueprintType)
struct FModioEditModParams
{
    GENERATED_BODY()

public:

    TOptional<FString> Name;
    TOptional<FString> Summary;
    TOptional<FString> NamePath;
    TOptional<bool> bVisible;
    TOptional<FString> Description;
    TOptional<FString> HomepageURL;
    TOptional<EModioMaturityFlags> MaturityFlags;
    TOptional<FString> MetadataBlob;
};