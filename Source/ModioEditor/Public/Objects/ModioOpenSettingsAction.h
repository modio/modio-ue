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
#include "Objects/ModioStaticExecutionBase.h"
#include "ModioOpenSettingsAction.generated.h"

/** Opens the Modio Settings section of Project Settings
 *		No argument required.
 *		NOTE: We could replace this with one that takes an argument for what settings section to go to, making it more generic.
 */
UCLASS()
class MODIOEDITOR_API UModioOpenSettingsAction : public UModioStaticExecutionBase
{
	GENERATED_BODY()

	virtual bool ExecuteAction(FString Args) const override;
};
