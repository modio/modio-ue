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
#include "ModioCommonImage.h"
#include "ModioCommonDynamicImage.generated.h"

class UModioCommonDynamicImageStyle;

/**
 * Dynamic Image that displays an image that can be dynamically updated
 * It supports styling through the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonDynamicImage : public UModioCommonImage
{
	GENERATED_BODY()

public:
	/**
	 * Sets the style of the Dynamic Image within the Mod.io Common UI styling system
	 * @param InStyle The style to set
	 */
	void SetStyle(TSubclassOf<UModioCommonDynamicImageStyle> InStyle);

protected:
	//~ Begin UUserWidget Interface
	virtual void NativePreConstruct() override;
	//~ End UUserWidget Interface

public:
	//~ Begin UWidget Interface
	virtual void SynchronizeProperties() override;
	//~ End UWidget Interface
};
