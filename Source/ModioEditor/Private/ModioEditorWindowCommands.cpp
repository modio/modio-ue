/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "ModioEditorWindowCommands.h"

#define LOCTEXT_NAMESPACE "FModioEditorWindowModule"

void FModioEditorWindowCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "Create & Upload", "Create and upload content to mod.io", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE