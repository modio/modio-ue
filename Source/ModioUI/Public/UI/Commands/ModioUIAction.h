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
#include "Delegates/DelegateCombinations.h"
#include "UObject/NoExportTypes.h"

#include "ModioUIAction.generated.h"

DECLARE_DYNAMIC_DELEGATE(FModioUIExecuteAction);

DECLARE_DYNAMIC_DELEGATE_RetVal(bool, FModioUICanExecuteAction);

DECLARE_DYNAMIC_DELEGATE_RetVal(bool, FModioUIIsActionButtonVisible);

/**
* Struct definition to store operations performed at the UI level
**/
USTRUCT(BlueprintType)
struct MODIOUI_API FModioUIAction
{
	GENERATED_BODY()
	
	/**
	* Stored property of the action to execute
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UIActions")
	FModioUIExecuteAction ExecuteAction;

	/**
	* Stored property to query if the action can execute, an strong bool type
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UIActions")
	FModioUICanExecuteAction CanExecuteAction;

	/**
	* Stored property to query if the action button should display, an strong bool type
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UIActions")
	FModioUIIsActionButtonVisible IsActionVisible;
};
