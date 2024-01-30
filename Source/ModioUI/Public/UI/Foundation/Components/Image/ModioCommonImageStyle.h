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
#include "Templates/SubclassOf.h"
#include "Styling/SlateTypes.h"
#include "Templates/SharedPointer.h"
#include "ModioCommonImageStyle.generated.h"

/**
 * The style of the Image within the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonImageStyle : public UObject
{
	GENERATED_BODY()

public:
	/** Stored property to a default background brush */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Appearance")
	FSlateBrush ImageFrameBrush;

	/** Stored property to a default image brush */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Appearance")
	FSlateBrush ImageBrush;
};
