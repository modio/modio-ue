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
#include "UI/EventHandlers/IModioUIModInfoReceiver.h"
#include "UI/EventHandlers/IModioUIModManagementEventReceiver.h"
#include "UI/EventHandlers/IModioUISubscriptionsChangedReceiver.h"
#include "UI/EventHandlers/IModioUIUserChangedReceiver.h"
#include "UI/Foundation/Base/ModioCommonActivatableWidget.h"
#include "UI/Settings/Params/ModioCommonModBrowserParams.h"
#include "ModioCommonFeaturedPrimaryView.generated.h"

class UListView;
class UModioCommonFeaturedPrimaryLargeModEntry;
class UModioCommonFeaturedPrimaryViewStyle;

/**
 * @brief Featured Primary View that displays a list of mods in a large format
 * It supports styling through the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI5_API UModioCommonFeaturedPrimaryView
	: public UModioCommonActivatableWidget,
	  public IModioUIModInfoReceiver,
	  public IModioUIUserChangedReceiver,
	  public IModioUISubscriptionsChangedReceiver,
	  public IModioUIModManagementEventReceiver
{
	GENERATED_BODY()

public:
	UModioCommonFeaturedPrimaryView();

public:
	/**
	 * @brief Sets the style of the Featured Primary View within the Mod.io Common UI styling system
	 * @param InStyle The style to set
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Style")
	void SetStyle(UPARAM(DisplayName = "Style") TSubclassOf<UModioCommonFeaturedPrimaryViewStyle> InStyle);

protected:
	/** @brief The style of the Featured Primary View within the Mod.io Common UI styling system */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), DisplayName = "Style", Category = "Mod.io Common UI")
	TSubclassOf<UModioCommonFeaturedPrimaryViewStyle> ModioStyle;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonFeaturedPrimaryLargeModEntry> LargeModEntry;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, MustImplement = "ModioCommonModListViewInterface"), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UListView> ModList;

protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void FetchPrimaryCategoryMods();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void DisplayModsMatchingFilter(const FModioFilterParams& Filter);
	
public:
	/**
	 * @brief Is any mod selected in the list
	 * @return true if any mod is selected
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	bool IsAnyModSelected();

	/**
	 * @brief Clears the selection of selected mods in the list
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void ClearModSelection();

	virtual void SynchronizeProperties() override;
protected:
	virtual void NativeOnInitialized() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	virtual void NativeOnListAllModsRequestCompleted(FString RequestIdentifier, FModioErrorCode ErrorCode, TOptional<FModioModInfoList> List) override;

	virtual void NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent) override;

private:
	FModioFilterParams CurrentFilter;
};