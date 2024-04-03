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
#include "Widgets/SWindow.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Templates/SharedPointer.h"
#include "Types/ModioGameInfo.h"

#include "IDetailCustomization.h"

class FModioSettingsDetails : public IDetailCustomization
{

public:

	// Makes a new instance of this detail layout class for a specific detail view requesting it
	static TSharedRef<IDetailCustomization> MakeInstance();

	// IDetailCustomization interface
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailLayout) override;
	// End of IDetailCustomization interface

	/** @brief The root window we must reference, close, and dereference. */
	TSharedPtr<SWindow> ConfigWindow;

	/** @brief Stores the root content box for the authentication/configuration widgets */
	TSharedPtr<SVerticalBox> ConfigContentBox;

	/** @brief Stores the Game ID Input for capturing into settings. */
	TSharedPtr<SEditableTextBox> GameIdInput;

	/** @brief Stores the API Key Input for capturing into settings. */
	TSharedPtr<SEditableTextBox> ApiKeyInput;

	/** @brief Selected game from User's game list */
	FModioGameInfo SelectedGame;

	/** @brief Clears all content in the config box. */
	void ClearConfigContent();

	/** @brief Draws a throbber widget to denote async operations. */
	void DrawThrobber();

	/** @brief Draws user authentication dialog */
	void DrawConfigGuide();

	/** @brief Called when the user clicks the configure button, opening the configuration guide window. */
	FReply OnConfigureClicked();

	/** @brief called the user "submits" their settings, applying them to the settings module. */
	FReply OnSettingsSubmittedByUser();

	/** @brief Applys the given settings to ModioSettings and closes the window. */
	void ApplySettingsAndClose();
};
