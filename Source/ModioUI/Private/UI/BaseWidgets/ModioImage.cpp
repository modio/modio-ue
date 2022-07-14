// Fill out your copyright notice in the Description page of Project Settings.

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
	if (Texture)
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
	if (UMaterialInstanceDynamic* Instance = Cast<UMaterialInstanceDynamic>(Material))
	{
		CachedMaterial = Instance;
	}
}
