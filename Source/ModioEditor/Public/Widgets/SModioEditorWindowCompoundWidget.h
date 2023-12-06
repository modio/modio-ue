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

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include <Types/ModioGameInfo.h>
#include <Widgets/DeclarativeSyntaxSupport.h>
#include <Brushes/SlateDynamicImageBrush.h>
#include <Widgets/SBoxPanel.h>
#include <Widgets/Text/STextBlock.h>
#include <Widgets/Input/SEditableTextBox.h>
#include <Widgets/Images/SThrobber.h>
#include <Widgets/Input/SButton.h>
#include <Widgets/Notifications/SProgressBar.h>
#include <Widgets/Images/SImage.h>

class UModioSubsystem;
class UModioBrowseModsObject;
struct FModioModInfo;
struct FModioModID;
struct FModioErrorCode;
struct FModioBrowseModFileStruct;
struct FModioModManagementEvent;

/**
 * @brief A compound widget class used for Modio Editor Functionality to upload and edit mods with detail customizations.
 */
class MODIOEDITOR_API SModioEditorWindowCompoundWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SModioEditorWindowCompoundWidget)
	{}
	SLATE_END_ARGS()

	/** @brief Stored property to a ModioSubsystem pointer loaded by ModioSubsystem.cpp */
	UModioSubsystem* ModioSubsystem;

	/** @brief Stored property to a game information such as game name, description etc. */
	TOptional<FModioGameInfo> ModioGameInfo;

	/** @brief Stored property to the game thumbnail for which the ModioSubsystem is initialized. */
	TSharedPtr<SImage> ModioGameLogo;

	/** @brief Stored property to all PNG brushes in Resources directory. */
	TMap<FString, FSlateDynamicImageBrush*> TexturePool;

	/** @brief Stored property to a vertical box container that will show game information at the top of the widget. */
	TSharedPtr<SVerticalBox> GameInfoVerticalBoxList;

	/** @brief Stored property to a vertical box container that will show other widgets below the game information. */
	TSharedPtr<SVerticalBox> VerticalBoxList;

	/** @brief Stored property to the game name text block. */
	TSharedPtr<STextBlock> GameName;

	/** @brief Stored property to the UGC name text block. */
	TSharedPtr<STextBlock> UGCName;

	/** @brief Stored property to the description text block. */
	TSharedPtr<STextBlock> Description;

	/** @brief Stored property to the email text box. */
	TSharedPtr<SEditableTextBox> ModioEmailEditableTextBox;

	/** @brief Stored property to the authentication code text box. */
	TSharedPtr<SEditableTextBox> ModioAuthenticationCodeEditableTextBox;

	/** @brief Stored property to the throbber widget when a submit button is pressed. */
	TSharedPtr<SCircularThrobber> SubmitThrobber;

	/** @brief Stored property to the progress bar that shows the compress and upload progress on the mod file. */
	TSharedPtr<SProgressBar> ProgressBar;

	/** @brief Stored property to the progress bar title. */
	TSharedPtr<STextBlock> ProgressTitle;

	/** @brief Stored property to the percentage text block. */
	TSharedPtr<STextBlock> PercentageText;

	/** @brief Stored property to the submit button on upload mod file widget. */
	TSharedPtr<SButton> ModfileSubmitButton;

	/** @brief Stored property to the back button on upload mod file widget. */
	TSharedPtr<SButton> ModfileBackButton;

	/** @brief The id of the mod that is being uploaded. */
	FModioModID UploadModID;

	/** @brief Percentage progress of the upload mod file. */
	float Percentage;

	/** @brief Path to the root directory of resources. */
	FString ResourcesPath;

	/** @brief Path to the current game's logo in resources directory locally. */
	FString GameLogoPath;

	/** @brief Stored property to the game information background brush at the top. */
	FSlateBrush* HeaderBackgroundBrush;

	/** @brief Stored property to the main widget's background brush. */
	FSlateBrush* BackgroundBrush;

	/** @brief Stored property to a large text style used for headers. */
	FSlateFontInfo HeaderLargeTextStyle;

	/** @brief Stored property to a small text style used for normal texts. */
	FSlateFontInfo HeaderSmallTextStyle;

	/** @brief Stored property to a text style used for buttons. */
	FSlateFontInfo ButtonTextStyle;

	void Construct(const FArguments& InArgs);
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

	/** @brief Loads and instantiates resources such as brushes and other values. */
	void LoadResources();

	/** @brief Sets the ModioSubsystem for initialization. */
	void LoadModioSubsystem();

	void EnableModManagement();
	void OnModManagementCallback(FModioModManagementEvent Event);

	/**
	 * @brief Callback to determine if Modio initialization was successful.
	 */
	void OnInitCallback(FModioErrorCode ErrorCode);

	/**
	 * @brief Determines if a mod.io user is already logged in, on the current platform device.
	 * @param ErrorCode An error code with a value of 0 if a user is already logged in.
	 */
	void OnVerifyCurrentUserAuthenticationCompleted(FModioErrorCode ErrorCode);

	/** @brief Draws the 'login widget' on the window client. */
	void DrawLoginWidget();
	FReply OnLoginButtonClicked();

	/**
	 * @brief Determines if an authentication code is sent successfully.
	 * @param ErrorCode An error code with a value of 0 if an authentication code is sent.
	 */
	void OnRequestEmailAuthCodeCompleted(FModioErrorCode ErrorCode);

	/** @brief Draws the 'authentication widget' on the window client. */
	void DrawAuthenticateWidget();
	FReply OnAuthenticateButtonClicked();

	/**
	 * @brief Determines if the authentication completed successfully.
	 * @param ErrorCode An error code with a value of 0 if the authentication completed successfully.
	 */
	void OnAuthCodeCompleted(FModioErrorCode ErrorCode);

	/** @brief Removes widgets from the window client. */
	void ClearAllWidgets();

	/** @brief Draws the 'logo widget' on the window client. */
	void DrawLogoWidget();

	/** @brief Draws the 'circular throbber' widget on the window client. */
	void DrawThrobberWidget();

	/** @brief Draws the mod creation tool widget on the window client. */
	void DrawModCreationToolWidget();

	/**
	 * @brief Draws the 'browse mods widget' on the window client.
	 * @param ModInfoList An array of mods passed for detail customization.
	 */
	void DrawBrowseModsWidget(TArray<FModioModInfo> ModInfoList);

	/**
	 * @brief Draws the 'edit mod widget' on the window client.
	 * @param BrowseModsProperties Contains a list of mods for detail customization.
	 */
	void DrawEditModWidget(UModioBrowseModsObject* BrowseModsProperties);

	/**
	 * @brief Draws the 'create mod file tool widget' on the window client.
	 * @param ModID A mod id is required to create a mod file for the mod.
	 */
	void DrawCreateModFileToolWidget(FModioModID ModID);

	/**
	 * @brief Draws the 'create or edit mod file widget' on the window client.
	 * @param ModID A mod id is required to create or edit a mod file for the mod.
	 * @param BrowseModFileObject A browse mod file object is passed for detail customization.
	 */
	void DrawCreateOrEditModFileWidget(FModioModID ModID, FModioBrowseModFileStruct BrowseModFileObject);

	/**
	 * @brief Draws the 'browse mod file widget' on the window client.
	 * @param ModID A mod id is required to show the mod file(s) for the mod.
	 */
	void DrawBrowseModFileWidget(FModioModInfo ModID);
	
	/** @brief Draws the progress bar on the window client when a file is submitted for compress and upload. */
	void ShowProgressBar();

	/** @brief Updates the progress bar per frame based on the progress. */
	void UpdateProgressBar();

	/** @brief Hides the progress bar widget from the window client. */
	void HideProgressBar();

	/**
	 * @brief Logs out a user from the mod.io in the current platform device.
	 * @param Exit Determines if the window client should exit.
	 */
	void Logout(bool Exit = false);

	/** @brief Unloads resources and other objects whenever a request for close window is called. */
	void UnloadResources();

	/**
	 * @brief Localizes a specified string.
	 * @param Key A key for the localization text.
	 * @param Text The actual string to localize.
	 * @returns Returns a localized FText.
	 */
	FText Localize(FString Key, FString Text);

	/**
	 * @brief Gets a font info based on the specified params.
	 * @param PropertyName The name of the font.
	 * @param FaceName The face name of the font.
	 * @param Size The size of the font.
	 * @returns Returns FSlateFontInfo object.
	 */
	FSlateFontInfo GetTextStyle(FName PropertyName, FName FaceName, int32 Size);

	/**
	 * @brief Downloads the game thumbnails from the URL received by GetGameInfoAsyn function.
	 * @param URL The url of the game's thumbnail.
	 */
	void DownloadGameLogo(FString URL);

	FString ToNonPlural(const FString& String);

	EModioModfilePlatform ToPlatformEnum(FString Platform);
	FString ToPlatformString(EModioModfilePlatform Platform);
	bool DoesGameSupportThisPlatform(FModioGameInfo GameInfo, FModioGamePlatform& GamePlatform);
};
