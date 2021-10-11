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
#include "Types/ModioModInfoList.h"

#include "ModioModInfoListLibrary.generated.h"


UCLASS()
class MODIO_API UModioModInfoListLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/**
	 * Get the mods in a mod info list
	 * @param ModInfoList
	 * @return
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities")
	static const TArray<FModioModInfo>& GetMods(const FModioModInfoList& ModInfoList);

	/**
	 * Get the paged result that contains information of the data returned
	 * @param ModInfoList
	 * @return
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities")
	static const FModioPagedResult& GetPagedResult(const FModioModInfoList& ModInfoList);
};
