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
#include "UI/Commands/ModioUIAction.h"
#include "UI/Commands/ModioUIMenuEntry.h"
#include "UObject/NoExportTypes.h"

#include "ModioUIMenuCommandList.generated.h"

/**
* Strong type struct to store the list of actions to perform for a 
* corresponding entry in the UI menu
**/
USTRUCT(BlueprintType)
struct MODIOUI_API FModioUIMenuCommandList
{
	GENERATED_BODY()

	/**
	* Dictionary that stores a entry with an action to perform
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|Menu Commands")
	TMap<FModioUIMenuEntry, FModioUIAction> MappedActions;
};
