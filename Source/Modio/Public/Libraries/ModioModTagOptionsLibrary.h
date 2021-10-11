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
#include "Types/ModioModTagOptions.h"

#include "ModioModTagOptionsLibrary.generated.h"


UCLASS()
class MODIO_API UModioModTagOptionsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/**
	 * Get the tags in a mod tag options list
	 * @param ModTags 
	 * @return
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities")
	static const TArray<FModioModTagInfo>& GetTags(const FModioModTagOptions& ModTags);

	/**
	 * Get the paged result that contains information of the data returned
	 * @param ModTags
	 * @return
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities")
	static const FModioPagedResult& GetPagedResult(const FModioModTagOptions& ModTags);
};
