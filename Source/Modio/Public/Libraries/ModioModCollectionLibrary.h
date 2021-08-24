#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Types/ModioModCollectionEntry.h"

// clang-format off
#include "ModioModCollectionLibrary.generated.h"
// clang-format on

UCLASS()
class UModioModCollectionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/** @return EModioModState enum representing current state of the mod */
	UFUNCTION(BlueprintPure, Category = "mod.io|Mods")
	static EModioModState GetModState(const FModioModCollectionEntry& Entry);

	/** @return Mod ID */
	UFUNCTION(BlueprintPure, Category = "mod.io|Mods")
	static FModioModID GetID(const FModioModCollectionEntry& Entry);

	/** @return FModioModInfo containing mod profile data */
	UFUNCTION(BlueprintPure, Category = "mod.io|Mods")
	static const FModioModInfo& GetModProfile(const FModioModCollectionEntry& Entry);

	/**
	 * @return Path to the mod's installation folder on disk
	 * NOTE: If the mod is not yet installed this path may not yet exist. Check
	 * xref:GetModState[] before trying to load files in this location
	 **/
	UFUNCTION(BlueprintPure, Category = "mod.io|Mods")
	static const FString& GetPath(const FModioModCollectionEntry& Entry);
};
