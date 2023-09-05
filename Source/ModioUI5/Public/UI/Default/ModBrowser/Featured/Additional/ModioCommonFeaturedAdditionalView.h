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
#include "ModioCommonFeaturedAdditionalView.generated.h"

class UListView;
class UModioCommonTabButtonBase;
class UModioCommonTabListWidgetBase;
class UModioCommonFeaturedAdditionalViewStyle;

/**
 * @brief Featured Additional View that displays an additional view for the Featured View
 * It supports styling through the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI5_API UModioCommonFeaturedAdditionalView
	: public UModioCommonActivatableWidget,
	  public IModioUIModInfoReceiver,
	  public IModioUIUserChangedReceiver,
	  public IModioUISubscriptionsChangedReceiver,
	  public IModioUIModManagementEventReceiver
{
	GENERATED_BODY()

public:
	UModioCommonFeaturedAdditionalView();

public:
	/**
	 * @brief Sets the style of the Featured Additional View within the Mod.io Common UI styling system
	 * @param InStyle The style to set
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Style")
	void SetStyle(UPARAM(DisplayName = "Style") TSubclassOf<UModioCommonFeaturedAdditionalViewStyle> InStyle);

protected:
	/** @brief The style of the Featured Additional View within the Mod.io Common UI styling system */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), DisplayName = "Style", Category = "Mod.io Common UI")
	TSubclassOf<UModioCommonFeaturedAdditionalViewStyle> ModioStyle;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, MustImplement = "ModioCommonModListViewInterface"), Category = "Mod.io Common UI")
	TObjectPtr<UListView> ModList;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI")
	UModioCommonTabListWidgetBase* CategoryTabList;

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

public:
	virtual void SynchronizeProperties() override;

protected:
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	
public:
	void RefreshList(const FModioFilterParams& Filter);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;
	virtual void NativeOnListAllModsRequestCompleted(FString RequestIdentifier, FModioErrorCode ErrorCode, TOptional<FModioModInfoList> List) override;

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent) override;

protected:
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Mod.io Common UI")
	FModioFilterParams CurrentFilter;
};
