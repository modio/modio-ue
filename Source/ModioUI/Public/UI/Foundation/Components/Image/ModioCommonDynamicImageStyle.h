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
#include "Misc/EngineVersionComparison.h"
#if UE_VERSION_OLDER_THAN(5, 3, 0)
	#include "SlateCore/Public/Styling/SlateBrush.h"
#else
	#include "Styling/SlateBrush.h"
#endif
#include "Templates/SubclassOf.h"
#include "ModioCommonDynamicImageStyle.generated.h"

/**
 * The style of the Dynamic Image within the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonDynamicImageStyle : public UModioCommonImageStyle
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	FSlateBrush ImageLoadingBrush;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	FSlateBrush ImageUnavailableBrush;
};
