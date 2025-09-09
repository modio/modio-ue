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
#include "Types/ModioEditModParams.h"
#include "Types/ModioModInfo.h"
#include "Widgets/ModioEditorUploadAndManageUGCWidget.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/SModioEditorWindowCompoundWidget.h"

/**
 *
 */
class MODIOEDITOR_API SModioEditorEditModProfileWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SModioEditorEditModProfileWidget) {}
	SLATE_ARGUMENT(SModioEditorWindowCompoundWidget*, ParentWindow)
	SLATE_ARGUMENT(FModioModInfo, ModInfo)
	SLATE_EVENT(SModioEditorUploadAndManageUGCWidget::FOnNavigate, NavigationHandler)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs, UModioSubsystem* InModioSubsystem);

private:
	bool bIsDirty;
	bool bIsLoading;
	UModioSubsystem* ModioSubsystem;

	FModioEditModParams EditModParams;

	SModioEditorWindowCompoundWidget* ParentWindow;
	SModioEditorUploadAndManageUGCWidget::FOnNavigate NavigationHandler;
	FModioModInfo ModInfo;

	TSharedPtr<STextComboBox> VisibilityComboBox;
	TSharedPtr<SBox> ContentBox;
	TSharedPtr<SGridPanel> MaturityGridPanel;

	TArray<TSharedPtr<FString>> ModVisibilityOptions;

	void OnModVisibilityChanged(TSharedPtr<FString>, ESelectInfo::Type SelectInfo);
	void OnThumbnailPathPicked(const FString& PickedPath);
	FString GetThumbnailPath() const;
	void UpdateWindowContent();

	void PopulateModMaturityOptions(TSharedPtr<SGridPanel> Grid);

	TSharedPtr<SWidget> CreateLabeledCheckbox(const TTuple<FText, EModioMaturityFlags>& Option, bool bCheck);
};
