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
 * @brief Dynamic Image that displays an image that can be dynamically updated
 * It supports styling through the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI5_API UModioCommonDynamicImage : public UModioCommonImage
{
	GENERATED_BODY()

public:
	void SetStyle(TSubclassOf<UModioCommonDynamicImageStyle> InStyle);

protected:
	virtual void NativePreConstruct() override;
public:
	virtual void SynchronizeProperties() override;
};
