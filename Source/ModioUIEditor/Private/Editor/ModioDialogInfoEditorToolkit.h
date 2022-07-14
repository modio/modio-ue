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
#include "Toolkits/AssetEditorToolkit.h"
#include "UObject/StrongObjectPtr.h"

/// @brief Custom editor toolkit for editing the data objects for data-driven modal dialogs
class FModioDialogEditorToolkit : public FAssetEditorToolkit
{
	const FName DialogEditorAppId = FName("ModioDialogEditorApp");
	const FName DialogPreviewTabId = FName("ModioDialogEditor_Preview");
	const FName DialogPropertiesTabId = FName("ModioDialogEditor_Properties");
	TSharedPtr<class SOverlay> DialogPreview;
	TSharedPtr<class IDetailsView> DialogPropertyEditor;
	TStrongObjectPtr<class UModioDialogBaseInternal> DialogWidget;
	UObject* ObjectBeingEdited = nullptr;

public:
	void CreateInternalWidgets();
	void UpdatePreview(const FPropertyChangedEvent&);

	TSharedRef<SDockTab> HandleTabSpawnerSpawnPreview(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> HandleTabSpawnerSpawnProperties(const FSpawnTabArgs& Args);
	using FAssetEditorToolkit::InitAssetEditor;
	virtual void InitAssetEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost,
						 UObject* ObjectToEdit);

	void RegisterTabSpawners(const TSharedRef<FTabManager>& TabManager) override;

	void UnregisterTabSpawners(const TSharedRef<FTabManager>& TabManager) override;

	FName GetToolkitFName() const override;

	FText GetBaseToolkitName() const override;

	FString GetWorldCentricTabPrefix() const override;

	FLinearColor GetWorldCentricTabColorScale() const override;

};