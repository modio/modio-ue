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
#include "ModioCommonImageStyle.h"
#include "SlateCore/Public/Styling/SlateBrush.h"
#include "Templates/SubclassOf.h"
#include "ModioCommonDynamicImageStyle.generated.h"

/**
 * @brief The style of the Dynamic Image within the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI5_API UModioCommonDynamicImageStyle : public UModioCommonImageStyle
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	FSlateBrush ImageLoadingBrush;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	FSlateBrush ImageUnavailableBrush;
};
