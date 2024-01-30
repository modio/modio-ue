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
#include "Components/ExpandableArea.h"
#include "Templates/SubclassOf.h"
#include "Templates/SharedPointer.h"
#include "Styling/SlateTypes.h"
#include "Widgets/SOverlay.h"
#include "ModioCommonExpandableArea.generated.h"

class UModioCommonTextBlock;
class UModioCommonExpandableAreaStyle;

/**
 * Expandable area widget that allows to customize the style
 */
UCLASS(ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonExpandableArea : public UExpandableArea
{
	GENERATED_BODY()

public:
	/**
	 * Set the style of the Expandable Area within the Mod.io Common UI styling system
	 * @param InStyle The style to set
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Style")
	void SetStyle(UPARAM(DisplayName = "Style") TSubclassOf<UModioCommonExpandableAreaStyle> InStyle);

protected:
	/** The style of the Expandable Area within the Mod.io Common UI styling system */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), DisplayName = "Style", Category = "Mod.io Common UI")
	TSubclassOf<UModioCommonExpandableAreaStyle> ModioStyle;

public:
	/**
	 * Handle the focus received event
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI")
	void HandleOnFocusReceived();

	/**
	 * Handle the focus lost event
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI")
	void HandleOnFocusLost();
private:
	bool bIsFocused = false;

	//~ Begin UWidget Interface
public:
	virtual void SynchronizeProperties() override;
protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	//~ End UWidget Interface

	/**
	 * Get the attached label text block
	 * This is a helper function to get the text block that is attached to the header of the expandable area
	 * @param AttachedTextBlock Found text block
	 * @return true if found, false otherwise
	 */
	virtual bool GetAttachedLabelTextBlock(UModioCommonTextBlock*& AttachedTextBlock);

	/**
	 * Refresh the content of the expandable area. This forces the widget to be rebuilt and reapply the style
	 * @return The new content of the expandable area
	 */
	virtual TSharedRef<SWidget> RefreshContent();

	/**
	 * Refresh the style of the expandable area
	 */
	virtual void RefreshStyle();

protected:
	/** Widget that wraps the expandable area. Needed to properly apply the style with the built slate widget */
	TSharedPtr<SOverlay> WrapperWidget;

	/** Cached style to avoid unnecessary refreshes */
	UPROPERTY(Transient)
	TSubclassOf<UModioCommonExpandableAreaStyle> CachedAppliedModioStyle;
};
