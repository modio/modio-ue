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
#include "CommonTextBlock.h"
#include "ModioCommonTextBlock.generated.h"

class UModioCommonTextStyle;

/**
 * Text Block styled using the Mod.io Common UI styling system
 */
UCLASS(ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonTextBlock : public UCommonTextBlock
{
	GENERATED_BODY()

public:
	/**
	 * Sets the style of the Text Block within the Mod.io Common UI styling system
	 * @param InStyle The style to set
	 */
	void SetStyle(TSubclassOf<UModioCommonTextStyle> InStyle);
};
