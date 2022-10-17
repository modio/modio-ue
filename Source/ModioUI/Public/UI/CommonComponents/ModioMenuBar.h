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

#include "UI/BaseWidgets/ModioUserWidgetBase.h"
#include "UI/CommonComponents/ModioRichTextButton.h"
#include "UI/CommonComponents/ModioUserProfileButton.h"

#include "ModioMenuBar.generated.h"

/**
* Modio UI element that represents a menu bar. It contains 
* references to a seach, back and profile button
**/
UCLASS()
class MODIOUI_API UModioMenuBar : public UModioUserWidgetBase
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;
	virtual void SynchronizeProperties() override;

	UFUNCTION()
	virtual void OnBackClicked();

	UFUNCTION()
	virtual void OnSearchClicked();

	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget))
	UModioRichTextButton* SearchButton;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget))
	UModioRichTextButton* BackButton;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget))
	UModioUserProfileButton* UserProfileButton;

public:
	// These functions are dirty but it is either that or subclass the tab switcher widget and this is marginally
	// cleaner than that
	/**
	* Update the visibility status for the search button
	* @param bVisible True to make the button visible, false otherwise
	**/
	void SetSearchButtonVisibility(bool bVisible);
	
	/**
	 * Update the visibility status for the back button
	 * @param bVisible True to make the button visible, false otherwise
	 **/
	void SetBackButtonVisibility(bool bVisible);
	
	/**
	 * Update the visibility status for the profile button
	 * @param bVisible True to make the button visible, false otherwise
	 **/
	void SetProfileButtonVisibility(bool bVisible);
};
