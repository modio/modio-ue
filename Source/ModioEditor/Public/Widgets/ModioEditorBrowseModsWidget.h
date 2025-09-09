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
#include "Widgets/ModioEditorUploadAndManageUGCWidget.h"
#include "Widgets/SCompoundWidget.h"
#include "types/ModioModInfo.h"

/**
 *
 */
class MODIOEDITOR_API SModioEditorBrowseModsWidget : public SCompoundWidget
{
public:
	DECLARE_DELEGATE_OneParam(FOnModSelected, TSharedPtr<FModioModInfo> ModInfo);

	SLATE_BEGIN_ARGS(SModioEditorBrowseModsWidget) {}
	SLATE_EVENT(FOnModSelected, ModSelectedHandler)
	SLATE_EVENT(SModioEditorUploadAndManageUGCWidget::FOnNavigate, NavigationHandler)
	SLATE_ARGUMENT(SModioEditorUploadAndManageUGCWidget*, UploadAndManageWidget)
	SLATE_ARGUMENT(SModioEditorWindowCompoundWidget*, ParentWindow)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs, UModioSubsystem* InModioSubsystem);

private:
	void DrawModList();

	void FilterModList();

	FOnModSelected ModSelectedHandler;

	TSharedPtr<SVerticalBox> MainContent;
	TSharedPtr<SBox> ModListBox;
	TSharedPtr<SListView<TSharedPtr<FModioModInfo>>> ListView;

	TSharedRef<ITableRow> GenerateModInfoRow(TSharedPtr<FModioModInfo> ModInfo,
											 const TSharedRef<STableViewBase>& OwnerTable);

	void OnModSelected(TSharedPtr<FModioModInfo> Item, ESelectInfo::Type SelectInfoType);

	const FText* FilterString;
	TArray<TSharedPtr<FModioModInfo>> ModInfoOriginalSource;
	TArray<TSharedPtr<FModioModInfo>> ModInfoFilteredSource;

	UModioSubsystem* ModioSubsystem;
};
