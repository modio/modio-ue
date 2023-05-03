/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidgets/ModioImage.h"
#include "UI/Styles/ModioUIBrushRef.h"

#include "ModioRoundedImage.generated.h"

/**
* Modio UI element that represents a single rounded image
**/
UCLASS()
class MODIOUI_API UModioRoundedImage : public UModioImage
{
	GENERATED_BODY()
protected:
	virtual void SynchronizeProperties() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	FModioUIMaterialRef MaterialToUse;

	UPROPERTY(Transient)
	UMaterialInterface* CachedReferencedMaterial;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UMaterialInstanceDynamic* ImageMaterial;


	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	FName TextureParameterName = "WidgetTexture";

	virtual void LoadImageFromFileAsync(FModioImageWrapper ImageLoader) override;
};
