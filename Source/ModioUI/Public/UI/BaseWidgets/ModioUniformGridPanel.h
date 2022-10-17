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

#include "Components/UniformGridPanel.h"
#include "CoreMinimal.h"
#include "ModioUniformGridPanel.generated.h"

/**
* Class definition for a Modio Uniform grid panel. It evenly divides available
* space between its child elements
**/
UCLASS()
class MODIOUI_API UModioUniformGridPanel : public UUniformGridPanel
{
	GENERATED_BODY()
};
