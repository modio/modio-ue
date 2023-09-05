/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Default/Gallery/ModioCommonModGalleryEntry.h"

#include "UI/Default/Gallery/ModioCommonModGalleryEntryStyle.h"
#include "UI/Default/Gallery/ModioCommonModGalleryView.h"
#include "UI/Foundation/Components/Image/ModioCommonDynamicImage.h"
#include "UI/Foundation/Components/Image/ModioCommonDynamicImageStyle.h"
#include "UI/Foundation/Components/Image/ModioCommonImage.h"

void UModioCommonModGalleryEntry::SetStyle(TSubclassOf<UModioCommonModGalleryEntryStyle> InStyle)
{
	ModioStyle = InStyle;
	SynchronizeProperties();
}

void UModioCommonModGalleryEntry::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (UModioCommonModGalleryEntryStyle* StyleCDO = ModioStyle.GetDefaultObject())
	{
		if (ModImage)
		{
			if (TSubclassOf<UModioCommonDynamicImageStyle> ImageStyle = StyleCDO->ModImageStyle)
			{
				ModImage->SetStyle(ImageStyle);
			}
		}
	}
}

void UModioCommonModGalleryEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	if (UModioCommonModGalleryItem* GalleryItem = Cast<UModioCommonModGalleryItem>(ListItemObject))
	{
		if (ModImage)
		{
			if (GalleryItem->ImageGalleryIndex == -1)
			{
				ModImage->LoadImageFromLogo(GalleryItem->ModInfo.ModId, EModioLogoSize::Thumb320);
			}
			else
			{
				ModImage->LoadImageFromGallery(GalleryItem->ModInfo.ModId, EModioGallerySize::Thumb320, GalleryItem->ImageGalleryIndex);
			}
		}
	}
}
