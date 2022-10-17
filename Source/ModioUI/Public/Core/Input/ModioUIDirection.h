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

#include "ModioUIDirection.generated.h"

/**
* Enumerator with possible move action
**/
UENUM()
enum class EModioUIDirection : uint8
{
	/** Perform a move left action **/
	MoveLeft,

	/** Perform a move right action **/
	MoveRight,

	/** Perform a move up action **/
	MoveUp,

	/** Perform a move down action **/
	MoveDown
};