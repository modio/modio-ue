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
#include "Components/ProgressBar.h"
#include "ModioCommonProgressBar.generated.h"

class UModioCommonProgressBarStyle;

/**
 * Progress Bar that can be used in Mod.io UI
 * It supports styling through the Mod.io Common UI styling system
 */
UCLASS(ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonProgressBar : public UProgressBar
{
	GENERATED_BODY()

public:
	/**
	 * Sets the style of the Progress Bar within the Mod.io Common UI styling system
	 * @param InStyle The style to set
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Style")
	void SetStyle(UPARAM(DisplayName = "Style") TSubclassOf<UModioCommonProgressBarStyle> InStyle);

protected:
	/** The style of the Progress Bar within the Mod.io Common UI styling system */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), DisplayName = "Style", Category = "Mod.io Common UI")
	TSubclassOf<UModioCommonProgressBarStyle> ModioStyle;

public:
	//~ Begin UProgressBar Interface
	virtual void SynchronizeProperties() override;
	//~ End UProgressBar Interface
};
