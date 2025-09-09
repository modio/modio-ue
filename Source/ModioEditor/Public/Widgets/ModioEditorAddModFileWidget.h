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

#include "ModioSubsystem.h"
#include "Types/ModioCreateModFileParams.h"
#include "Types/ModioModInfo.h"
#include "Widgets/Layout/SGridPanel.h"
#include "Widgets/ModioEditorUploadAndManageUGCWidget.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/SModioEditorWindowCompoundWidget.h"

/**
 *
 */
class MODIOEDITOR_API SModioEditorAddModFileWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SModioEditorAddModFileWidget) {}
	SLATE_ARGUMENT(SModioEditorWindowCompoundWidget*, ParentWindow)
	SLATE_ARGUMENT(FModioModInfo, ModInfo)
	SLATE_EVENT(SModioEditorUploadAndManageUGCWidget::FOnNavigate, NavigationHandler)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs, UModioSubsystem* InModioSubsystem);

private:
	bool bAcceptedTerms;

	FModioCreateModFileParams CreateModFileParams;

	SModioEditorWindowCompoundWidget* ParentWindow;
	UModioSubsystem* ModioSubsystem;
	FModioModInfo ModInfo;

	SModioEditorUploadAndManageUGCWidget::FOnNavigate NavigationHandler;

	TSharedPtr<SGridPanel> PlatformGridPanel;
	TSharedPtr<SBox> ContentBox;

	TSharedPtr<SWidget> CreateLabeledCheckbox(const TTuple<FText, EModioModfilePlatform>& Info);
	void PopulatePlatformOptions(TSharedPtr<SGridPanel> Grid);

	void UpdateWindowContent();

	void OnModRootPathChosen(const FString& PickedPath);
};
