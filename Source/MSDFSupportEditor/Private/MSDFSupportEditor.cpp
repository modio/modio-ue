/* 
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *  
 *  This file is part of the mod.io UE4 Plugin.
 *  
 *  Distributed under the MIT License. (See accompanying file LICENSE or 
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *   
 */

#include "MSDFSupportEditor.h"
#include "AssetToolsModule.h"

DEFINE_LOG_CATEGORY(MSDFSupportEditor);

#define LOCTEXT_NAMESPACE "FMSDFSupportEditor"

void FMSDFSupportEditor::StartupModule()
{
}

void FMSDFSupportEditor::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FMSDFSupportEditor, MSDFSupportEditor)