// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Views/ModDetails/ModioModDetailsImage.h"
#include "Engine/Texture2DDynamic.h"

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
	}
}
