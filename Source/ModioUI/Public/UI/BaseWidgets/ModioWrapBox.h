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

#include "Components/WrapBox.h"
#include "CoreMinimal.h"

#include "ModioWrapBox.generated.h"

/**
* Class definition for a Modio wrap box, which helps arrange widgets 
* left-to-right or top-to-bottom according to a set orientation
**/
UCLASS()
class MODIOUI_API UModioWrapBox : public UWrapBox
{
	GENERATED_BODY()
};
