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

#include "Types/ModioCommonTypes.h"
#include "Types/ModioModInfo.h"

#include "ModioModCollectionEntry.generated.h"



namespace Modio
{
	class ModCollectionEntry;
}

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

/** @brief Class representing a mod which is installed locally */
USTRUCT(BlueprintType)
struct MODIO_API FModioModCollectionEntry
{
	GENERATED_BODY()

	EModioModState GetModState() const;

	FModioModID GetID() const;

	const FModioModInfo& GetModProfile() const;

	const FString GetPath() const;

private:
	friend struct FModioModCollectionEntry ToUnreal(const class Modio::ModCollectionEntry& In);
	
	EModioModState ModState;
	FModioModID ModID;
	TOptional<FString> ModPath;
	FModioModInfo ModProfile;
};
