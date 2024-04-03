/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "CoreMinimal.h"
#include "UI/Foundation/Base/Search/ModioCommonSearchViewBase.h"
#include "Types/ModioFilterParams.h"
#include "UI/EventHandlers/IModioUIModInfoReceiver.h"
#include "ModioCommonSearchTabView.generated.h"

class UModioCommonTextBlock;
class UModioCommonSearchTabViewStyle;
class UModioCommonBorder;
class UModioCommonTabButtonBase;
class UModioCommonFilteringView;
class UModioCommonEditableTextBox;
class UModioCommonTabListWidgetBase;
class UModioCommonWidgetSwitcher;
class UModioCommonButtonBase;

/**
 * Search Tab View that allows the user to search for mods using the search tags, filtering and sorting
 * It supports styling through the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonSearchTabView : public UModioCommonSearchViewBase
{
	GENERATED_BODY()

public:
	/**
	 * Sets the style of the Search Tab View within the Mod.io Common UI styling system
	 * @param InStyle The style to set
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Style")
	void SetStyle(UPARAM(DisplayName = "Style") TSubclassOf<UModioCommonSearchTabViewStyle> InStyle);

protected:
	/** The style of the Search Tab View within the Mod.io Common UI styling system */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), DisplayName = "Style", Category = "Mod.io Common UI")
	TSubclassOf<UModioCommonSearchTabViewStyle> ModioStyle;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonBorder> InternalBackgroundBorder;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonBorder> OverlayBackgroundBorder;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonTextBlock> SearchTabTitleTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonFilteringView> FilteringView;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Search Tab View|Widgets")
	TObjectPtr<UModioCommonEditableTextBox> SearchTextBox;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Search Tab View|Widgets")
	TObjectPtr<UModioCommonButtonBase> CloseButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Search Tab View|Widgets")
	TObjectPtr<UModioCommonButtonBase> SearchButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Search Tab View|Widgets")
	TObjectPtr<UModioCommonButtonBase> ResetButton;

protected:
	//~ Begin UModioCommonActivatableWidget Interface
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	virtual void NativeOnSetDataSource() override;
	//~ End UModioCommonActivatableWidget Interface

	//~ Begin UUserWidget Interface
	virtual void NativeOnInitialized() override;
	//~ End UUserWidget Interface
public:
	//~ Begin UWidget Interface
	virtual void SynchronizeProperties() override;
	//~ End UWidget Interface
protected:

	UFUNCTION()
	void OnSearchTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Search Tab View")
	void ShowSearchResults();

public:
	/**
	 * Resets the search tab view, clearing the search text and resetting the filtering options to the default values
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Search Tab View")
	void Reset();

	/**
	 * Zeros out the search tab view, clearing the search text and zeroing out the filtering options
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Search Tab View")
	void ZeroOut();

	/**
	 * Gets the currently selected filter params
	 * @return The currently selected filter params
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Search Tab View")
	FModioFilterParams GetFilterParams() const;

	/**
	 * Gets the filter params wrapper for the filtering options (easier to modify in Blueprints)
	 * @return The filter params wrapper
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Search Tab View")
	FModioModCategoryParams GetFilterParamsWrapper() const;
};
