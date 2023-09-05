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

class UModioCommonSearchTabViewStyle;
class UModioCommonBorder;
class UModioCommonTabButtonBase;
class UModioCommonFilteringView;
class UModioCommonSortingView;
class UModioCommonEditableTextBox;
class UModioCommonTabListWidgetBase;
class UModioCommonWidgetSwitcher;
class UModioCommonButtonBase;

/**
 * @brief Search Tab View that allows the user to search for mods using the search tags, filtering and sorting
 * It supports styling through the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI5_API UModioCommonSearchTabView : public UModioCommonSearchViewBase
{
	GENERATED_BODY()

public:
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Transient, Category = "Mod.io Common UI|Preview")
	bool bPreviewDisplaySortingOrFiltering = true;
#endif

public:
	/**
	 * @brief Sets the style of the Search Tab View within the Mod.io Common UI styling system
	 * @param InStyle The style to set
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Style")
	void SetStyle(UPARAM(DisplayName = "Style") TSubclassOf<UModioCommonSearchTabViewStyle> InStyle);

protected:
	/** @brief The style of the Search Tab View within the Mod.io Common UI styling system */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), DisplayName = "Style", Category = "Mod.io Common UI")
	TSubclassOf<UModioCommonSearchTabViewStyle> ModioStyle;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonBorder> InternalBackgroundBorder;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonBorder> OverlayBackgroundBorder;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonTabListWidgetBase> SortingFilteringTabList;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonWidgetSwitcher> SortingFilteringContentSwitcher;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Search Tab View|Widgets")
	TObjectPtr<UModioCommonEditableTextBox> SearchTextBox;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Search Tab View|Widgets")
	TObjectPtr<UModioCommonButtonBase> CloseButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Search Tab View|Widgets")
	TObjectPtr<UModioCommonButtonBase> SearchButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Search Tab View|Widgets")
	TObjectPtr<UModioCommonButtonBase> ClearAllButton;

protected:
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Mod.io Common UI|Search Tab View|Widgets")
	TObjectPtr<UModioCommonSortingView> SortingView;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "Mod.io Common UI|Search Tab View|Widgets")
	TObjectPtr<UModioCommonFilteringView> FilteringView;

protected:
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
public:
	virtual void SynchronizeProperties() override;
protected:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Search Tab View")
	bool GetViewFromTabNameID(FName TabNameID, UModioCommonActivatableWidget*& OutView) const;

	UFUNCTION()
	void OnSearchTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Search Tab View")
	void ShowSearchResults();

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Search Tab View")
	void ClearAllFilters();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Search Tab View")
	FModioFilterParams GetFilterParams() const;

private:
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Mod.io Common UI|Search Tab View", meta = (AllowPrivateAccess = true))
	FName SelectedTabID;
};
