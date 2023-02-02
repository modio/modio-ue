/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/Views/ModDetails/ModioModDetailsImage.h"
#include "Engine/Texture2DDynamic.h"
#include "Materials/MaterialInterface.h"

void UModioModDetailsImage::NativeDisplayImage(const FSlateBrush& InImage)
{
	if (Image)
	{
		if (UTexture2DDynamic* Texture = Cast<UTexture2DDynamic>(InImage.GetResourceObject()))
		{
			if (UMaterialInterface* ResolvedMaterial = FrameMaterial.ResolveReference())
			{
				Image->DisplayImageWithMaterial(Texture, ResolvedMaterial, TextureParameterName);
			}
		}
		else
		{
			if (UMaterialInterface* Material = Cast<UMaterialInterface>(InImage.GetResourceObject())) {
				Image->SetBrushFromMaterial(Material);
			}
		}
	}
}
