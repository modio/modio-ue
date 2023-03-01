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

#include "Components/Widget.h"
#include "ModioWidgetBase.generated.h"

/**
* Modio UI element that represents the base class to other Modio Widget instances
**/
UCLASS()
class MODIOUI_API UModioWidgetBase : public UWidget
{
	GENERATED_BODY()
};