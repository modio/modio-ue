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
#include "Widgets/Input/SEditableTextBox.h"
#include "Templates/SharedPointer.h"

#include "Types/ModioErrorCode.h"
#include "ModioSubsystem.h"

/**
 *
 */
class MODIOEDITOR_API SModioEditorUserAuthWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SModioEditorUserAuthWidget)
		{}
	SLATE_END_ARGS()

	/** @brief Stored property to a ModioSubsystem pointer loaded by ModioSubsystem.cpp */
	UModioSubsystem* ModioSubsystem;

	/** @brief Stored property to a vertical box container that will show other widgets, stored for easy clearing. */
	TSharedPtr<SVerticalBox> RootWidget;

	/** @brief Stored property to the email text box. */
	TSharedPtr<SEditableTextBox> ModioEmailEditableTextBox;

	/** @brief Stored property to the authentication code text box. */
	TSharedPtr<SEditableTextBox> ModioAuthenticationCodeEditableTextBox;

	/** @brief Final result of the authentication attempt. */
	FModioErrorCode AuthenticationResult;

	/** @brief Delegate called upon completion of this login/auth widget's work. Check for Errors and close this widet on success. */
	FOnErrorOnlyDelegateFast OnAuthenticationComplete;

	/** @brief Sets the ModioSubsystem for initialization. */
	void LoadModioSubsystem();

	/** @brief Callback to determine if Modio initialization was successful */
	void OnInitCallback(FModioErrorCode ErrorCode);

	void Construct(const FArguments& InArgs);
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

	/** @brief Unloads resources and other objects whenever a request for close window is called. */
	void UnloadResources();

	/** @brief Removes widgets from the window client. */
	void ClearAllWidgets();

	/** @brief Draws the 'circular throbber' widget on the window client. */
	void DrawThrobberWidget();

	/** @brief Called after we check if the user is already logged in once the Modio Subsyastem is initialized */
	void OnUserAuthCheckResponse(FModioErrorCode ErrorCode);

	/** @brief Draws the login with email widget, requesting the user enter their email for authentication. */
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

};
