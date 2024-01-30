/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "ModioUIEditor.h"

DEFINE_LOG_CATEGORY(ModioUIEditor);

#define LOCTEXT_NAMESPACE "FModioUIEditor"

void FModioUIEditor::StartupModule()
{
}

void FModioUIEditor::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FModioUIEditor, ModioUIEditor)