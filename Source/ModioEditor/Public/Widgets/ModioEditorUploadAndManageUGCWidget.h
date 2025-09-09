/*
 *  Copyright (C) 2025 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "CoreMinimal.h"
#include "Types/ModioModInfo.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/SModioEditorWindowCompoundWidget.h"

class UModioSubsystem;

/**
 *
 */
class MODIOEDITOR_API SModioEditorUploadAndManageUGCWidget : public SCompoundWidget
{
public:
	enum EUploadAndManageUIState
	{
		BrowseMods,
		CreateMod,
		EditMod_EditProfile,
		EditMod_AddNewModFile,
		EditMod_AddNewSourceModFile
	};

	DECLARE_DELEGATE_OneParam(FOnNavigate, SModioEditorUploadAndManageUGCWidget::EUploadAndManageUIState);

	SLATE_BEGIN_ARGS(SModioEditorUploadAndManageUGCWidget) {}
	SLATE_ARGUMENT(SModioEditorWindowCompoundWidget*, ParentWindow)
	SLATE_END_ARGS()

	/** @brief Stored property to a ModioSubsystem pointer loaded by ModioSubsystem.cpp */

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	void DrawContent();

	const TSharedRef<SWidget> GetContent();

	TOptional<FModioModInfo> CurrentModInfo;

private:
	SModioEditorWindowCompoundWidget* ParentWindow;
	TSharedPtr<SBox> WidgetContent;

	UModioSubsystem* ModioSubsystem;
	EUploadAndManageUIState UploadAndEditUIState = BrowseMods;

	void HandleNavigation(EUploadAndManageUIState FromState);
	void ModSelectionChanged(TSharedPtr<FModioModInfo> ModInfo);

	void ShowProgressBar();
	void UpdateProgressBar();
	void HideProgressBar();
};
