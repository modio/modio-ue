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
#include "Input/CommonBoundActionBar.h"

#include "ModioCommonBoundActionBar.generated.h"

/**
 * Base class for widgets that display a bound action bar
 */
UCLASS(ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonBoundActionBar : public UCommonBoundActionBar
{
	GENERATED_BODY()
};
