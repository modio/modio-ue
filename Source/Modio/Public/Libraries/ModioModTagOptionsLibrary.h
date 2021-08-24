#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Types/ModioModTagOptions.h"

// clang-format off
#include "ModioModTagOptionsLibrary.generated.h"
// clang-format on

UCLASS()
class UModioModTagOptionsLibrary : public UBlueprintFunctionLibrary
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
