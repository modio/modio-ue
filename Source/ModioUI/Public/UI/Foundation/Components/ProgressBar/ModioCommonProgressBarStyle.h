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
#include "Styling/SlateTypes.h"
#include "Templates/SubclassOf.h"
#include "ModioCommonProgressBarStyle.generated.h"

/**
 * The style of the Progress Bar within the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonProgressBarStyle : public UObject
{
	GENERATED_BODY()

public:
	/** The brush used to draw the progress bar */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	FProgressBarStyle Style;
};
