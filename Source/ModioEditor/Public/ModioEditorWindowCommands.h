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

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "ModioEditorWindowStyle.h"

class FModioEditorWindowCommands : public TCommands<FModioEditorWindowCommands>
{
public:

	FModioEditorWindowCommands()
		: TCommands<FModioEditorWindowCommands>(TEXT("ModioEditorWindow"), NSLOCTEXT("Contexts", "ModioEditorWindow", "ModioEditorWindow Plugin"), NAME_None, FModioEditorWindowStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};