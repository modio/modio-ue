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
#include "CommonUserWidget.h"
#include "Types/ModioModInfo.h"
#include "UObject/ObjectPtr.h"

#include "ModioCommonImageGalleryItemBase.generated.h"

class UModioCommonImageBase;
class UModioCommonTextBlock;

/**
 * @brief Image Gallery Item Base class for image gallery items
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI5_API UModioCommonImageGalleryItemBase : public UCommonUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonImageBase> Image;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mod.io Common UI", meta = (ExposeOnSpawn = true))
	FModioModInfo ModInfo;
};