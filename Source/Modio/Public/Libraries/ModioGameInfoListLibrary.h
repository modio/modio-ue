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

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Types/ModioGameInfoList.h"

#include "ModioGameInfoListLibrary.generated.h"


UCLASS()
class MODIO_API UModioGameInfoListLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/**
	 * Get the games in a game info list
	 * @param GameInfoList
	 * @return
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities")
	static const TArray<FModioGameInfo>& GetGames(const FModioGameInfoList& GameInfoList);

	/**
	 * Get the paged result that contains information of the data returned
	 * @param GameInfoList
	 * @return
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities")
	static const FModioPagedResult& GetPagedResult(const FModioGameInfoList& GameInfoList);
};
