/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/BaseWidgets/ModioImage.h"
#include "Engine/Texture2DDynamic.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "Misc/EnumRange.h"

UModioImage::UModioImage(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void UModioImage::SynchronizeProperties()
{
	Super::SynchronizeProperties();
}

void UModioImage::LoadImageFromFileAsync(FModioImageWrapper ImageLoader)
{
	ImageLoader.LoadAsync(FOnLoadImageDelegateFast::CreateUObject(this, &UModioImage::ImageLoadHandler));
}

void UModioImage::ImageLoadHandler(UTexture2DDynamic* Texture)
{
	if (Texture)
	{
		CachedMaterial = nullptr;
		SetBrushFromTextureDynamic(Texture);
	}
}
void UModioImage::ImageLoadHandler(UTexture2DDynamic* Texture, UMaterialInterface* Material, FName ImageParameterName)
{
	if (Texture && IsValid(Material))
	{
		SetBrushFromMaterial(Material);
		CachedMaterial = GetDynamicMaterial();
		CachedMaterial->SetTextureParameterValue(ImageParameterName, Texture);
		CachedMaterial->SetScalarParameterValue("UseRenderTarget", 1);
		InvalidateLayoutAndVolatility();
	}
}

void UModioImage::LoadImageFromFileWithMaterialAsync(FModioImageWrapper ImageLoader, UMaterialInterface* Material,
													 FName ImageParameterName)
{
	ImageLoader.LoadAsync(
		FOnLoadImageDelegateFast::CreateUObject(this, &UModioImage::ImageLoadHandler, Material, ImageParameterName));
}

void UModioImage::DisplayImageWithMaterial(UTexture2DDynamic* Texture, UMaterialInterface* Material,
										   FName ImageParameterName)
{
	ImageLoadHandler(Texture, Material, ImageParameterName);
}

void UModioImage::SetBrushFromMaterial(UMaterialInterface* Material)
{
	Super::SetBrushFromMaterial(Material);
	UMaterialInstanceDynamic* Instance = Cast<UMaterialInstanceDynamic>(Material);
	if (IsValid(Instance))
	{
		CachedMaterial = Instance;
	}
}
