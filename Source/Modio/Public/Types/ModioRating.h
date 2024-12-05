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

#include "ModioRating.generated.h"

/**
* Enumerator with the rating a mod can receive
**/
UENUM(BlueprintType)
enum class EModioRating : uint8
{
	/** A neutral rating */
	Neutral,
	/** A positive rating */
	Positive,
	/** A negative rating */
	Negative
};