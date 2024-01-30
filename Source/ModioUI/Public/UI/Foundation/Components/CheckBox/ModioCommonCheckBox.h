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
#include "UObject/Object.h"
#include "Components/CheckBox.h"
#include "Templates/SubclassOf.h"
#include "Styling/SlateTypes.h"
#include "Templates/SharedPointer.h"
#include "ModioCommonCheckBox.generated.h"

class UModioCommonTextBlock;
class UModioCommonCheckBoxStyle;

/**
 * Check Box that can be used in Mod.io UI
 * It supports styling through the Mod.io Common UI styling system
 */
UCLASS(ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonCheckBox : public UCheckBox
{
	GENERATED_BODY()

public:
	UModioCommonCheckBox();

	/**
	 * Sets the style of the Check Box within the Mod.io Common UI styling system
	 * @param InStyle The style to set
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Style")
	void SetStyle(UPARAM(DisplayName = "Style") TSubclassOf<UModioCommonCheckBoxStyle> InStyle);

protected:
	/** The style of the Check Box within the Mod.io Common UI styling system */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), DisplayName = "Style", Category = "Mod.io Common UI")
	TSubclassOf<UModioCommonCheckBoxStyle> ModioStyle;

public:
	/**
	 * Sets the label of the Check Box
	 * @param InLabelText The label to set
	 */
	UFUNCTION(BlueprintCallable, Category = "Text")
	void SetLabel(const FText& InLabelText);

	/**
	 * Gets the label of the Check Box
	 * @return The label
	 */
	UFUNCTION(BlueprintCallable, Category = "Text")
	FText GetLabel() const;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnModioCommonCheckBoxStateChanged, bool);
	/**
	 * Delegate called when the check box state changes
	 */
	FOnModioCommonCheckBoxStateChanged OnModioCommonCheckBoxStateChanged;

protected:
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonTextBlock> LabelTextBlock;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), Category = "Mod.io Common UI")
	FText LabelText;
public:
	//~ Begin UWidget Interface
	virtual void SynchronizeProperties() override;
protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	//~ End UWidget Interface

public:
	//~ Begin UVisual Interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	//~ End UVisual Interface
protected:
	/**
	 * Get the label text block text
	 * @return The label text block text
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI")
	FText GetLabelTextBlockText() const;

	/**
	 * Get the content widget of the check box
	 * @return The content widget
	 */
	virtual TSharedRef<SWidget> GetContentWidget();

	/**
	 * Get the content padding of the check box
	 * @return The content padding
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	FMargin GetCheckboxContentPadding() const;

	/**
	 * Handle the check box state changed event
	 * @param bIsChecked The new state of the check box
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void HandleOnCheckStateChanged(bool bIsChecked);
};
