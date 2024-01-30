/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Foundation/Components/Image/ModioCommonImage.h"

#include "CommonLazyImage.h"
#include "UI/Foundation/Components/Image/ModioCommonImageStyle.h"

void UModioCommonImage::SetStyle(TSubclassOf<UModioCommonImageStyle> InStyle)
{
	if (InStyle && InStyle != ModioStyle)
	{
		ModioStyle = InStyle;
		SynchronizeProperties();
	}
}

void UModioCommonImage::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UModioCommonImage::NativeConstruct()
{
	Super::NativeConstruct();
	SynchronizeProperties();
}

void UModioCommonImage::SetImageTexture_Implementation(UTexture2DDynamic* Texture)
{
	Super::SetImageTexture_Implementation(Texture);
	if (Image && Texture)
	{
		LoadedImageTexture = Texture;
	}
	SynchronizeProperties();
}

void UModioCommonImage::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	if (Image)
	{
		if (ModioStyle)
		{
			if (UModioCommonImageStyle* StyleCDO = ModioStyle.GetDefaultObject())
			{
				if (ImageFrame)
				{
					ImageFrame->SetBrush(StyleCDO->ImageFrameBrush);
				}
				if (Image)
				{
					Image->SetBrush(StyleCDO->ImageBrush);
				}
			}
		}

		if (Image && LoadedImageTexture)
		{
			Image->SetBrushFromTextureDynamic(LoadedImageTexture);
		}
	}
	BP_SynchronizeProperties();
}

void UModioCommonImage::ReleaseSlateResources(bool bReleaseChildren)
{
	//LoadedImageTexture = nullptr;
	Super::ReleaseSlateResources(bReleaseChildren);
}
