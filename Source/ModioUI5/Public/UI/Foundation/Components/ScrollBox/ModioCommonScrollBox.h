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
#include "Components/ScrollBox.h"
#include "ModioCommonScrollBox.generated.h"

class UModioCommonScrollBoxStyle;

/**
 * @brief Scroll Box that can be used in Mod.io UI
 * It supports styling through the Mod.io Common UI styling system
 */
UCLASS(ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI5_API UModioCommonScrollBox : public UScrollBox
{
	GENERATED_BODY()

public:
	/**
	 * @brief Sets the style of the Scroll Box within the Mod.io Common UI styling system
	 * @param InStyle The style to set
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Style")
	void SetStyle(UPARAM(DisplayName = "Style") TSubclassOf<UModioCommonScrollBoxStyle> InStyle);

protected:
	/** @brief The style of the Scroll Box within the Mod.io Common UI styling system */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), DisplayName = "Style", Category = "Mod.io Common UI")
	TSubclassOf<UModioCommonScrollBoxStyle> ModioStyle;

public:
	//~ Begin UScrollBox Interface
	virtual void SynchronizeProperties() override;
protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	//~ End UScrollBox Interface

protected:
	/**
	 * @brief Called when the user scrolls the scroll box
	 * @param CurrentOffset The current scroll offset
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Mod.io Common UI|Scroll Box")
	void OnUserScrolledHandle(float CurrentOffset);

	/**
	 * @brief Updates navigation data based on the pending scroll offset
	 * @param PendingScrollOffset The pending scroll offset (may differ from the actual current scroll offset due to ongoing scrolling animation, if any)
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Mod.io Common UI|Scroll Box")
	void UpdateNavigationData(int32 PendingScrollOffset);

	/**
	 * @brief Handles custom navigation when the scroll box reaches its boundary during navigation
	 * @param InNavigation The navigation being handled
	 * @return The widget to navigate to
	 */
	UFUNCTION(BlueprintNativeEvent)
	UWidget* HandleCustomBoundaryNavigation(EUINavigation InNavigation);

	/** @brief User-defined navigation data to apply when the scroll box can no longer scroll. Relevant only if bScrollByNavigationInput is true */
	UPROPERTY(Instanced, BlueprintReadOnly, Category="Navigation")
	TObjectPtr<class UWidgetNavigation> UserDefinedNavigation;
};
