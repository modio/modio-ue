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

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ModioStaticExecutionBase.generated.h"

/** A simple object that exists only to execute a single action.
 *		Inherit from this and override ExecuteAction() to implement a new type of action
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class MODIOEDITOR_API UModioStaticExecutionBase : public UObject
{
	GENERATED_BODY()

public:

	UModioStaticExecutionBase();

	virtual bool ExecuteAction(FString Args) const;
};
