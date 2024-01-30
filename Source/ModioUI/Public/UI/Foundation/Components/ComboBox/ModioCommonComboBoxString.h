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
#include "Components/ComboBoxString.h"
#include "ModioCommonComboBoxString.generated.h"

class UModioCommonComboBoxStringStyle;

/**
 * Combo Box String that can be used in Mod.io UI
 * It supports styling through the Mod.io Common UI styling system
 */
UCLASS(ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonComboBoxString : public UComboBoxString
{
	GENERATED_BODY()

public:
	/**
	 * Sets the style of the Combo Box String within the Mod.io Common UI styling system
	 * @param InStyle The style to set
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Style")
	void SetStyle(UPARAM(DisplayName = "Style") TSubclassOf<UModioCommonComboBoxStringStyle> InStyle);

protected:
	/** The style of the Combo Box String within the Mod.io Common UI styling system */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), DisplayName = "Style", Category = "Mod.io Common UI")
	TSubclassOf<UModioCommonComboBoxStringStyle> ModioStyle;

public:
	//~ Begin UWidget Interface
	virtual void SynchronizeProperties() override;
protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	//~ End UWidget Interface

	//~ Begin UComboBoxString Interface
	virtual TSharedRef<SWidget> HandleGenerateWidget(TSharedPtr<FString> Item) const override;
	//~ End UComboBoxString Interface
};
