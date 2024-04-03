/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Views/SListView.h"

#include "Types/ModioGameInfo.h"
#include "ModioSubsystem.h"

class MODIOEDITOR_API SModioEditorUserGamesList : public SCompoundWidget
{

public:
	SLATE_BEGIN_ARGS(SModioEditorUserGamesList)
		{}
	SLATE_END_ARGS()

	/** @brief Stored property to a ModioSubsystem pointer loaded by ModioSubsystem.cpp */
	UModioSubsystem* ModioSubsystem;

	/** @brief Stored property for our root component box. */
	TSharedPtr<SVerticalBox> Root;

	/** @brief Delegate called when a game is selected form the list. Does not mean this widget is closing. */
	FOnGetGameInfoDelegateFast OnGameSelected;

	/** @brief Stored reference to SListView widget that is displaying the games. */
	TSharedPtr<SListView<TSharedPtr<FModioGameInfo>>> GamesListView;

	/** @brief Cached list of games for displaying. */
	TArray<TSharedPtr<FModioGameInfo>> Games;

	void Construct(const FArguments& InArgs);
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

	/** @brief Gets and stores the Modio Subsystem refernece. */
	void LoadModioSubsystem();

	/** @brief Called when we get a response to out ModioSubsystem initilisation attempt */
	void OnModioInitCallback(FModioErrorCode ErrorCode);

	/** @brief Clears all content from this widget's root. */
	void ClearWidget();

	/** @brief Draws a loading throbber to indicate async operations. */
	void DrawThrobber();

	/** @brief Gets and draws the game list. */
	void DrawGameList();

	/** @brief SListView's callback function for generating rows. */
	TSharedRef<ITableRow> GenerateGameInfoRow(TSharedPtr<FModioGameInfo> GameInfo,
											  const TSharedRef<STableViewBase>& OwnerTable);

	/** @brief Called by SListView when a game is selected. Fires a delegate with the relevant game's info. */
	void OnGameSelectedFromList(TSharedPtr<FModioGameInfo> SelectedGame, ESelectInfo::Type SelectInfo);
};
