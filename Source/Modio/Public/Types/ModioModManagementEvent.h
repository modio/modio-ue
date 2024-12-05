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
#include "Types/ModioErrorCode.h"

#include "ModioModManagementEvent.generated.h"

/** @brief What type of event occurred */
UENUM(BlueprintType)
enum class EModioModManagementEventType : uint8
{
	/** Mod installation to local storage completed*/
	Installed, 
	/** Mod uninstallation from local storage completed*/
	Uninstalled, 
	/** Mod local installation updated to latest version*/
	Updated, 
	/** Mod file was uploaded*/
	Uploaded, 
	/** Mod download and installation has started */
	BeginInstall, 
	/** Mod uninstallation has started */
	BeginUninstall, 
	/** Mod download and update has started*/
	BeginUpdate, 
	/** Mod upload has started*/
	BeginUpload
};

/** @brief Simple struct representing the outcome of a mod management operation */
USTRUCT(BlueprintType)
struct MODIO_API FModioModManagementEvent
{
	GENERATED_BODY()

	/** @brief ID for the mod that the event occurred on */
	UPROPERTY(BlueprintReadOnly, Category = "ModManagementEvent")
	FModioModID ID;

	/** @brief What type of event occurred */
	UPROPERTY(BlueprintReadOnly, Category = "ModManagementEvent")
	EModioModManagementEventType Event {};

	/** @brief Empty if operation completed successfully, truthy/contains error code if operation failed */
	UPROPERTY(BlueprintReadOnly, Category = "ModManagementEvent")
	FModioErrorCode Status;
};
