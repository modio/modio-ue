/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "AssetActions/AssetTypeActions_ModioDialogInfo.h"
#include "Editor/ModioDialogInfoEditorToolkit.h"
#include "UI/Dialogs/ModioDialogBaseInternal.h"
#include "UI/Dialogs/ModioDialogInfo.h"

UClass* FAssetTypeActions_ModioDialogInfo::GetSupportedClass() const
{
	return UModioDialogInfo::StaticClass();
}

uint32 FAssetTypeActions_ModioDialogInfo::GetCategories()
{
	return EAssetTypeCategories::UI;
}

void FAssetTypeActions_ModioDialogInfo::OpenAssetEditor(
	const TArray<UObject*>& InObjects,
	TSharedPtr<class IToolkitHost> EditWithinLevelEditor /*= TSharedPtr<IToolkitHost>()*/)
{
	EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;
	TSharedRef<FModioDialogEditorToolkit> DialogEditor(new FModioDialogEditorToolkit());
	DialogEditor->InitAssetEditor(Mode, EditWithinLevelEditor, InObjects[0]);
}
