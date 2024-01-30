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
#include "Styling/SlateTypes.h"
#include "Types/SlateEnums.h"
#include "ModioCommonScrollBarStyle.generated.h"

/**
 * The style of the Scroll Bar within the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonScrollBarStyle : public UObject
{
	GENERATED_BODY()

public:
	/** The brush used to draw the scrollbar */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	FScrollBarStyle Style;

	/** Whether the scrollbar should always be visible */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	bool bAlwaysShowScrollbar;

	/** Whether the scrollbar track should always be visible */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	bool bAlwaysShowScrollbarTrack;

	/** Orientation of the scrollbar */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	TEnumAsByte<EOrientation> Orientation;

	/** The thickness of the scrollbar thumb */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	FVector2D Thickness;

	/** The margin around the scrollbar */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	FMargin Padding;
};
