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
#include "Types/ModioCreateModParams.h"
#include "Widgets/Input/STextComboBox.h"
#include "Widgets/ModioEditorUploadAndManageUGCWidget.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/SModioEditorWindowCompoundWidget.h"

/**
 *
 */
class MODIOEDITOR_API SModioEditorCreateModProfileWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SModioEditorCreateModProfileWidget) {}
	SLATE_ARGUMENT(SModioEditorWindowCompoundWidget*, ParentWindow)
	SLATE_EVENT(SModioEditorUploadAndManageUGCWidget::FOnNavigate, NavigationHandler)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs, UModioSubsystem* InSubsystem);

private:
	bool bIsLoading;

	UModioSubsystem* ModioSubsystem;

	FModioCreateModParams CreateModParams;

	SModioEditorWindowCompoundWidget* ParentWindow;
	SModioEditorUploadAndManageUGCWidget::FOnNavigate NavigationHandler;

	TSharedPtr<SGridPanel> MaturityGridPanel;
	TSharedPtr<SGridPanel> TagsGridPanel;
	TSharedPtr<SBox> ContentBox;
	TSharedPtr<STextComboBox> VisibilityComboBox;

	TArray<TSharedPtr<FString>> ModVisibilityOptions;

	void OnModVisibilityChanged(TSharedPtr<FString>, ESelectInfo::Type SelectInfo);
	void OnThumbnailPathPicked(const FString& PickedPath);
	FString GetThumbnailPath() const;

	void UpdateWindowContent();
	void PopulateModMaturityOptions(TSharedPtr<SGridPanel> Grid);
	void PopulateModTagCheckboxes(TSharedPtr<SGridPanel> Grid);

	TSharedPtr<SWidget> CreateLabeledTagCheckbox(const FString& Label);
	TSharedPtr<SWidget> CreateLabeledMaturityCheckbox(const FText& Label, EModioMaturityFlags Flag);
};
