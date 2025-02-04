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

#include "Types/ModioCommonTypes.h"
#include "Types/ModioModInfo.h"
#include "Types/ModioUnsigned64.h"
#include "Types/ModioErrorCode.h"

#include "ModioModCollectionEntry.generated.h"

namespace Modio
{
	class ModCollectionEntry;
}

/// @brief Enum representing the current state of a mod
UENUM(BlueprintType)
enum class EModioModState : uint8
{
	// The mod is pending installation. This state is not saved.
	InstallationPending, 
	// The mod is installed.
	Installed,
	// The mod is pending an update. This state is saved as installed.
	UpdatePending, 
	// The mod is downloading as part of the installation process. This state is not saved.
	Downloading, 
	// The mod is extracting as part of the installation process. This state is not saved.
	Extracting, 
	// The mod is pending uninstallation. This state is saved as installed.
	UninstallPending, 
};

/**
 * @docpublic
 * @brief Struct representing a mod which is installed locally
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioModCollectionEntry
{
	GENERATED_BODY()

	/**
	 * @docpublic
	 * @brief Get the current mod state
	 * @return The state of the mod
	 */
	EModioModState GetModState() const;

	/**
	 * @docpublic
	 * @brief Get the unique identifier for the mod
	 * @return The mod's unique ID
	 */
	FModioModID GetID() const;

	/**
	 * @docpublic
	 * @brief Get the mod's profile information
	 * @return The mod's profile data
	 */
	const FModioModInfo& GetModProfile() const;

	/**
	 * @docpublic
	 * @brief Get the path where the mod is stored
	 * @return The path as a string
	 */
	const FString GetPath() const;

	/**
	 * @docpublic
	 * @brief Get the size of the mod on disk
	 * @return The size of the mod in bytes
	 */
	FModioUnsigned64 GetSizeOnDisk() const;

	/**
	 * @docpublic
	 * @brief Get the last error encountered when dealing with the mod
	 * @return The error code associated with the last operation
	 */
	FModioErrorCode GetLastError();

private:
	friend struct FModioModCollectionEntry ToUnreal(const class Modio::ModCollectionEntry& In);
	FModioUnsigned64 SizeOnDisk;
	EModioModState ModState;
	FModioModID ModID;
	TOptional<FString> ModPath;
	FModioModInfo ModProfile;
	FModioErrorCode LastError;
};
