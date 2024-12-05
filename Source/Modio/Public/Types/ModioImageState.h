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

#include "ModioImageState.generated.h"

/**
* Enumerator to represent the possible states the image data
* can be, from hard drive to corrupted
**/
UENUM(BlueprintType)
enum class EModioImageState : uint8
{
	/** Image data is located on hard drive */
	OnDisc,

	/** Image data is transferring to a memory location */
	LoadingIntoMemory,

	/** Image data is located in memory */
	InMemory,

	/** Image data is not readable */
	Corrupted
};