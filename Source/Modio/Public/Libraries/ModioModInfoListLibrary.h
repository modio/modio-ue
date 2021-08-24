#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Types/ModioModInfoList.h"

// clang-format off
#include "ModioModInfoListLibrary.generated.h"
// clang-format on

UCLASS()
class UModioModInfoListLibrary : public UBlueprintFunctionLibrary
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
