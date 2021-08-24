#pragma once

#include "ModioSDK.h"
#include "Internal/ModioPrivateDefines.h"
#include "Types/ModioCommonTypes.h"
#include "Types/ModioModInfo.h"

// clang-format off
#include "ModioModCollectionEntry.generated.h"
// clang-format on

/// @brief Enum representing the current state of a mod
UENUM(BlueprintType)
enum class EModioModState : uint8
{
	InstallationPending, // dont save
	Installed,
	UpdatePending, // saved as installed
	Downloading, // installing - dont save
	Extracting, // installing- don't save
	UninstallPending, // saved as installed
};

// @todo: This could just copy the four members that you can access
/** @brief Class representing a mod which is installed locally */
USTRUCT(BlueprintType)
struct MODIO_API FModioModCollectionEntry
{
	GENERATED_BODY()

	FModioModCollectionEntry() = default;
	FModioModCollectionEntry(Modio::ModCollectionEntry&& ModCollectionEntry);

	/** @return Modio::ModState enum representing current state of the mod */
	EModioModState GetModState() const;

	/** @return Mod ID */
	FModioModID GetID() const;

	/** @return Modio::ModInfo containing mod profile data */
	const FModioModInfo& GetModProfile() const;

	/**
	 * @return Path to the mod's installation folder on disk
	 * NOTE: If the mod is not yet installed this path may not yet exist. Check
	 * @doc_xref{ModCollectionEntry::GetModState} before trying to load files in this location
	 **/
	const FString& GetPath() const;

private:
	Modio::ModCollectionEntry ModCollectionEntry;

	/** Lazy init path, that we will initialize when we request the path */
	mutable TOptional<FString> CachedPath;

	/** Lazy init mod info, that we will initialize when we request the path */
	mutable TOptional<FModioModInfo> CachedModInfo;
};

#pragma region ToUnreal implementation
FORCEINLINE FModioModCollectionEntry ToUnreal(const FModioModCollectionEntry& ModCollectionEntry)
{
	return FModioModCollectionEntry(ModCollectionEntry);
}

MODIO_BEGIN_CONVERT_SWITCHES
FORCEINLINE EModioModState ToUnreal(Modio::ModState ModState)
{
	switch (ModState)
	{
		case Modio::ModState::InstallationPending:
			return EModioModState::InstallationPending;
		case Modio::ModState::Installed:
			return EModioModState::Installed;
		case Modio::ModState::UpdatePending:
			return EModioModState::UpdatePending;
		case Modio::ModState::Downloading:
			return EModioModState::Downloading;
		case Modio::ModState::Extracting:
			return EModioModState::Extracting;
		case Modio::ModState::UninstallPending:
			return EModioModState::UninstallPending;
	}

	checkf(false, TEXT("Missed a case in ToUnreal(Modio::ModState ModState)"));
	return EModioModState::InstallationPending;
}
MODIO_END_CONVERT_SWITCHES
#pragma endregion