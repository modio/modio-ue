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

#include "CoreMinimal.h"
#include "Misc/Crc.h"
#include "UObject/NoExportTypes.h"

#include "ModioUIMenuEntry.generated.h"

/**
* Strong type struct to represent a single entry in a UI menu
**/
USTRUCT(BlueprintType)
struct MODIOUI_API FModioUIMenuEntry
{
	GENERATED_BODY()

	/**
	* The text to display as part of this menu entry
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	FText MenuEntryLabel;

	/**
	* Comparator operator between menu entries
	**/
	bool operator==(const FModioUIMenuEntry& Other) const
	{
		return MenuEntryLabel.EqualTo(Other.MenuEntryLabel);
	}
};

/**
* Transform a menu entry into a unsigned 32 bit element
* @param Object the entry to transform
* @return UInt32 that represents the hash translation
**/
uint32 GetTypeHash(const FModioUIMenuEntry& Object);