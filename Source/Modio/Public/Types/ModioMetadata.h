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

#include "Containers/UnrealString.h"

#include "ModioMetadata.generated.h"

/**
* Strong type struct to represent key-value pairs for a mod metadata
**/
USTRUCT(BlueprintType)
struct MODIO_API FModioMetadata
{
	GENERATED_BODY()

	/** Key of the metadata */
	UPROPERTY(BlueprintReadOnly, Category = "Metadata")
	FString Key;

	/** Value of the metadata */
	UPROPERTY(BlueprintReadOnly, Category = "Metadata")
	FString Value;
};
