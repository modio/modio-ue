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
#include "UI/BaseWidgets/ModioRoundedImage.h"
#include "UI/BaseWidgets/ModioUserWidgetBase.h"
#include "UI/Interfaces/IModioUIImageDisplay.h"
#include "UI/Styles/ModioUIBrushRef.h"

#include "ModioModDetailsImage.generated.h"

/**
* Base class that stores details of a mod image: the rounded image pointer, 
* the material reference, and the texture name
**/
UCLASS()
class MODIOUI_API UModioModDetailsImage : public UModioUserWidgetBase, public IModioUIImageDisplay
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	UModioRoundedImage* Image;

	void NativeDisplayImage(const FSlateBrush& InImage);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Appearance")
	FModioUIMaterialRef FrameMaterial;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Appearance")
	FName TextureParameterName = "WidgetTexture";
};
