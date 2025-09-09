/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "Brushes/SlateDynamicImageBrush.h"
#include "CoreMinimal.h"
#include "Types/ModioGameInfo.h"
#include "Types/ModioUser.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Images/SThrobber.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Layout/SScaleBox.h"
#include "Widgets/Notifications/SProgressBar.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Text/STextBlock.h"

class UModioSubsystem;
class UModioBrowseModsObject;
struct FModioModInfo;
struct FModioModID;
struct FModioErrorCode;
struct FModioBrowseModFileStruct;
struct FModioModManagementEvent;

/**
 * @brief A compound widget class used for Modio Editor Functionality to upload and edit mods with detail
 * customizations.
 */
class MODIOEDITOR_API SModioEditorWindowCompoundWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SModioEditorWindowCompoundWidget) {}
	SLATE_END_ARGS()

	DECLARE_DELEGATE(FOnBackPressed);

	/** @brief Stored property to a ModioSubsystem pointer loaded by ModioSubsystem.cpp */
	UModioSubsystem* ModioSubsystem;

	/** @brief Stored property to a game information such as game name, description etc. */
	TOptional<FModioGameInfo> ModioGameInfo;

	/** @brief Stored property to the game thumbnail for which the ModioSubsystem is initialized. */
	TSharedPtr<SImage> ModioGameLogo;

	/** @breif Stored property to the game banner container */
	TSharedPtr<SScaleBox> ModioGameLogoContainer;

	/** @brief Stored property to all PNG brushes in Resources directory. */
	TMap<FString, FSlateDynamicImageBrush*> TexturePool;

	/** @brief Stored property to loaded brushes from assets */
	TMap<TSoftObjectPtr<UObject>, FSlateImageBrush> TextureAssetPool;

	/** @brief Stored property to a vertical box container that will show game information at the top of the widget. */
	TSharedPtr<SVerticalBox> GameInfoVerticalBoxList;

	/** @brief Stored property to a vertical box container that will show other widgets below the game information. */
	TSharedPtr<SVerticalBox> VerticalBoxList;

	/** @brief Box that contains the progress bar for mod uploading / compressing */
	TSharedPtr<SBox> ProgressBarBox;

	/** @brief Stored property to a vertical box container that will show the categories linked to from the landing
	 * page. */
	TSharedPtr<SVerticalBox> LandingCategoryList;

	/** @brief Stored property to the game name text block. */
	TSharedPtr<STextBlock> GameName;

	/** @brief Stored property to the UGC name text block. */
	TSharedPtr<STextBlock> UGCName;

	/** @brief Stored property to the description text block. */
	TSharedPtr<STextBlock> Description;

	/** @brief Stored property to the throbber widget when a submit button is pressed. */
	TSharedPtr<SCircularThrobber> SubmitThrobber;

	/** @brief Stored property to the progress bar that shows the compress and upload progress on the mod file. */
	TSharedPtr<SProgressBar> ProgressBar;

	/** @brief Stored property to the progress bar title. */
	TSharedPtr<STextBlock> ProgressTitle;

	/** @brief Stored property to the percentage text block. */
	TSharedPtr<STextBlock> PercentageText;

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

	FSlateImageBrush* LoginButtonBrush;

	/** @brief Stored property to the background brush used to distiguish content panels. */
	FSlateBrush* PanelBackgroundBrush;

	/** @brief Stored property to the main widget's background brush. */
	FSlateBrush* BackgroundBrush;

	/** @brief Stored property to a large text style used for headers. */
	FSlateFontInfo HeaderLargeTextStyle;

	/** @brief Stored property to a small text style used for normal texts. */
	FSlateFontInfo HeaderSmallTextStyle;

	/** @brief Stored property to a text style used for buttons. */
	FSlateFontInfo ButtonTextStyle;

	/** @brief Brush for a bolder seperator */
	FSlateBrush* BoldSeperatorBrush;

	/** @brief Separation Between Panel elements */
	float PanelPadding = 2.5f;

	/** @brief Content Padding on bottom navigation buttons */
	float BottomButtonPadding = 12;

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

	void DrawToolLanding();
	void DrawSubwindowWidget(TSharedPtr<SWidget> Widget);

	FReply OnLoginLandingButtonClicked();

	/** @brief Draws the 'login widget' on the window client. */
	void DrawLoginWidget();
	FReply OnLoginButtonClicked();

	/** @brief Draws the 'authenticate widget' on the window client. */
	FReply OnAlreadyHaveCodeClicked();

	/** @brief Draws the 'login widget' on the window client. */
	FReply OnUseDifferentEmailClicked();

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
	void DownloadGameImage(FString URL, FString SaveAs, FSimpleDelegate OnComplete);

	FString ToNonPlural(const FString& String);

	EModioModfilePlatform ToPlatformEnum(FString Platform);
	FString ToPlatformString(EModioModfilePlatform Platform);

	TSharedPtr<SWidget> CreateToolEntryWidget(struct FModioToolWindowEntry Entry);
};
