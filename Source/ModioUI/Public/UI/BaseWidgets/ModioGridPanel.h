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

#include "Components/GridPanel.h"
#include "CoreMinimal.h"

#include "ModioGridPanel.generated.h"

/**
* Modio UI element that displays content in a table-like panel that retains
* the width of every column throughout the table
**/
UCLASS()
class MODIOUI_API UModioGridPanel : public UGridPanel
{
	GENERATED_BODY()
};
