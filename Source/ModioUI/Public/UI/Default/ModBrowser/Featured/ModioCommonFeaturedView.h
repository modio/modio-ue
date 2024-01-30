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
#include "UI/EventHandlers/IModioUIModManagementEventReceiver.h"
#include "UI/EventHandlers/IModioUISubscriptionsChangedReceiver.h"
#include "UI/EventHandlers/IModioUIUserChangedReceiver.h"
#include "UI/Foundation/Base/ModBrowser/ModioCommonModListBase.h"
#include "ModioCommonFeaturedView.generated.h"

class UModioCommonFeaturedViewStyle;
class UModioCommonActivatableWidget;
class UModioCommonSearchResultsView;
class UModioCommonTabButtonBase;
class UModioCommonTabListWidgetBase;
class UModioCommonButtonBase;

/**
 * Featured View that displays mods in a featured view
 * It supports styling through the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonFeaturedView
	: public UModioCommonModListBase,
	  public IModioUIUserChangedReceiver,
	  public IModioUISubscriptionsChangedReceiver,
	  public IModioUIModManagementEventReceiver
{
	GENERATED_BODY()

public:
	UModioCommonFeaturedView();

	/**
	 * Sets the style of the Featured View within the Mod.io Common UI styling system
	 * @param InStyle The style to set
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Style")
	void SetStyle(UPARAM(DisplayName = "Style") TSubclassOf<UModioCommonFeaturedViewStyle> InStyle);

protected:
	/** The style of the Featured View within the Mod.io Common UI styling system */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), DisplayName = "Style", Category = "Mod.io Common UI")
	TSubclassOf<UModioCommonFeaturedViewStyle> ModioStyle;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonSearchResultsView> SearchResultsView;

public:
	virtual void SynchronizeProperties() override;
protected:
	virtual void NativeOnSetDataSource() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	virtual void NativeOnInitialized() override;

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Mod.io Common UI")
	void RefreshList();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Mod.io Common UI")
	void RefreshListByFilter(const FModioFilterParams& Filter);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Mod.io Common UI")
	void RefreshListByCategoryName(FName InCategoryName);

protected:
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Mod.io Common UI")
	FModioFilterParams PreviouslyAppliedFilter;

	UPROPERTY(Transient, BlueprintReadWrite, Category = "Mod.io Common UI")
	int64 OverriddenModsCount = INDEX_NONE;

	UPROPERTY(Transient, BlueprintReadWrite, Category = "Mod.io Common UI")
	bool bWasFilterAppliedAtLeastOnce = false;
};
