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
#include "Widgets/SBoxPanel.h"
#include "Widgets/Images/SImage.h"

#include "Templates/SharedPointer.h"
#include "Misc/Optional.h"

#include "Styling/SlateBrush.h"
#include "Brushes/SlateDynamicImageBrush.h"

#include "Types/ModioErrorCode.h"
#include "ModioSubsystem.h"

/**
 * 
 */
class MODIOEDITOR_API SModioEditorGameInfoWidget : public SCompoundWidget
{

public:
	SLATE_BEGIN_ARGS(SModioEditorGameInfoWidget)
		: _GameId(TOptional<FModioGameID>())
		{
		}
	/** GameId to query info for. Optional. */
	SLATE_ARGUMENT(TOptional<FModioGameID>, GameId)
	SLATE_END_ARGS()

	/** @brief Stored property to a ModioSubsystem pointer loaded by ModioSubsystem.cpp */
	UModioSubsystem* ModioSubsystem;

	/** @brief Stored property to a vertical box container that will show other widgets, stored for easy clearing. */
	TSharedPtr<SVerticalBox> RootWidget;

	/** @brief The given game we want this widget to display info about. Initialized via Construct Args. If null will look in ModioSetting. */
	TOptional<FModioGameID> GameId;

	/** @brief Stored property to a game information such as game name, description etc. */
	TSharedPtr<FModioGameInfo> LoadedGameInfo;

	/** @brief Stored property to the game thumbnail for which the ModioSubsystem is initialized. */
	TSharedPtr<SImage> LoadedGameLogo;

	/** @brief Stored property to all PNG brushes in Resources directory. */
	TMap<FString, FSlateDynamicImageBrush*> TexturePool;

	/** @brief Path to the root directory of resources. */
	FString ResourcesPath;

	/** @brief The default image size for a brush/game logo */
	const FVector2D DefaultImageSize = FVector2D(48.f, 48.f);

	/** @brief The expected (forced) size of a downloaded Game Logo */
	const FVector2D GameLogoSize = FVector2D(320.f, 180.f);

	/** @brief Sets the ModioSubsystem for initialization. */
	void LoadModioSubsystem();

	/** @brief Callback to determine if Modio initialization was successful
	* @param ErrorCode An error code for the init attempt.
	*/
	void OnInitCallback(FModioErrorCode ErrorCode);

	void Construct(const SModioEditorGameInfoWidget::FArguments& InArgs);
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

	/** @brief Removes widgets from the window client. */
	void ClearAllWidgets();

	/** @brief Draws the 'circular throbber' widget on the window client. */
	void DrawThrobberWidget();

	/** @brief Loads and instantiates resources such as brushes and other values. */
	void LoadResources();

	/** @brief Unloads resources and other objects whenever a request for close window is called. */
	void UnloadResources();

	/** @brief Called after we check if the user is already logged in once the Modio Subsyastem is initialized.
	* @param ErrorCode An error code for the Authentication attempt.
	*/
	void OnUserAuthCheckResponse(FModioErrorCode ErrorCode);

	/** @brief Querys the ModioSubsystem for Game Info belonging to the given GameId, or the GameId in ModioSettings if none is given.
	* @param GameId An optional FModioGameID variable.
	*/
	void LoadGameInfo();

	/**
	 * @brief Downloads the game thumbnails from the URL received by GetGameInfoAsyn function.
	 * @param URL The url of the game's thumbnail.
	 */
	void DownloadGameLogo(FString URL);

	/** @brief Called after receiving a response from the Mod.io subsystem after requesting game info.
	* @param ErrorCode An error code for the Get Game Indo attempt.
	* @param GameInfo The returned Game Info from the API call. May be null.
	*/
	void OnLoadGameInfoResponse(FModioErrorCode ErrorCode, TOptional<FModioGameInfo> GameInfo);

	/** @brief Draw proper game info the the widget. */
	void DrawGameInfo();

	/** @brief Draw a widget indicating game info is incorrect. */
	void DrawGameInfoInvalid(FModioErrorCode ErrorCode);
};
