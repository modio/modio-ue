// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/BaseWidgets/ModioUserWidgetBase.h"
#include "UI/CommonComponents/ModioRichTextButton.h"
#include "UI/CommonComponents/ModioUserProfileButton.h"

#include "ModioMenuBar.generated.h"

/**
 *
 */
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

	UPROPERTY(BlueprintReadOnly,Category="Widgets", meta = (BindWidget))
	UModioRichTextButton* SearchButton;

	UPROPERTY(BlueprintReadOnly,Category="Widgets", meta = (BindWidget))
	UModioRichTextButton* BackButton;

	UPROPERTY(BlueprintReadOnly,Category="Widgets", meta = (BindWidget))
	UModioUserProfileButton* UserProfileButton;

public:
	// These functions are dirty but it is either that or subclass the tab switcher widget and this is marginally
	// cleaner than that
	void SetSearchButtonVisibility(bool bVisible);
	void SetBackButtonVisibility(bool bVisible);
	void SetProfileButtonVisibility(bool bVisible);
};
