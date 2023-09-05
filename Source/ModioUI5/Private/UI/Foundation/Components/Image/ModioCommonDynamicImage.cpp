/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Foundation/Components/Image/ModioCommonDynamicImage.h"

#include "CommonLazyImage.h"
#include "Engine/Texture2DDynamic.h"
#include "UI/Foundation/Components/Image/ModioCommonDynamicImageStyle.h"

void UModioCommonDynamicImage::SetStyle(TSubclassOf<UModioCommonDynamicImageStyle> InStyle)
{
	TSubclassOf<UModioCommonImageStyle> CommonStyle = InStyle;
	Super::SetStyle(MoveTemp(CommonStyle));
}

void UModioCommonDynamicImage::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (!LoadedImageTexture)
	{
		OnStartLoadNewImage();
	}
}

void UModioCommonDynamicImage::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	if (Image && ModioStyle)
	{
		if (UModioCommonDynamicImageStyle* StyleCDO = Cast<UModioCommonDynamicImageStyle>(ModioStyle.GetDefaultObject()))
		{
			if (bLoadFailed)
			{
				Image->SetBrush(StyleCDO->ImageUnavailableBrush);
			}
			if (bIsLoading)
			{
				Image->SetBrush(StyleCDO->ImageLoadingBrush);
			}
		}
	}
	BP_SynchronizeProperties();
}
