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
	// The mod is pending installation.  This state is not saved.
	InstallationPending, 
	// The mod is installed.
	Installed,
	// The mod is pending an update. This state is saved as installed.
	UpdatePending, 
	// The mod is downloading as part of the installation process. This state is not saved.
	Downloading, 
	// The mod is extracting as part of the installation process. This state is not saved.
	Extracting, 
	// The mod is pending uninstallation.  This state is saved as installed.
	UninstallPending, 
};

/**
* Struct representing a mod which is installed locally 
**/
USTRUCT(BlueprintType)
struct MODIO_API FModioModCollectionEntry
{
	GENERATED_BODY()

	EModioModState GetModState() const;

	FModioModID GetID() const;

	const FModioModInfo& GetModProfile() const;

	const FString GetPath() const;

	FModioUnsigned64 GetSizeOnDisk() const;

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
