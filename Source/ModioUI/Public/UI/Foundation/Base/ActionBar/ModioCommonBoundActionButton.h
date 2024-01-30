/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "CoreMinimal.h"
#include "Input/CommonBoundActionButton.h"

#include "ModioCommonBoundActionButton.generated.h"

/**
 * Bound Action Button that displays a button that is bound to a specific action
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonBoundActionButton : public UCommonBoundActionButton
{
	GENERATED_BODY()
	
};
