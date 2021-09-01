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

// clang-format off
#include "ModioImageState.generated.h"
// clang-format on

UENUM(BlueprintType)
enum class EModioImageState : uint8
{
	OnDisc,
	LoadingIntoMemory,
	InMemory,
	Corrupted
};