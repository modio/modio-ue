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

#include "Containers/UnrealString.h"

#include "ModioModTag.generated.h"


USTRUCT(BlueprintType)
struct MODIO_API FModioModTag
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "ModTag")
	FString Tag;
};
