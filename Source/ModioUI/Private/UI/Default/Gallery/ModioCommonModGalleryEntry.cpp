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

#include "ModioUI.h"
#include "UI/Default/Gallery/ModioCommonModGalleryEntryStyle.h"
#include "UI/Default/Gallery/ModioCommonModGalleryView.h"
#include "UI/Foundation/Components/Image/ModioCommonDynamicImage.h"
#include "UI/Foundation/Components/Image/ModioCommonDynamicImageStyle.h"
#include "UI/Foundation/Components/Image/ModioCommonImage.h"

void UModioCommonModGalleryEntry::SetStyle(TSubclassOf<UModioCommonModGalleryEntryStyle> InStyle)
{
	if (InStyle && InStyle != ModioStyle)
	{
		ModioStyle = InStyle;
		SynchronizeProperties();
	}
}

void UModioCommonModGalleryEntry::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	NativeUpdateStyling(IsListItemSelected());
}

void UModioCommonModGalleryEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	NativeUpdateStyling(IsListItemSelected());

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

void UModioCommonModGalleryEntry::NativeOnItemSelectionChanged(bool bIsSelected)
{
	IUserObjectListEntry::NativeOnItemSelectionChanged(bIsSelected);
	NativeUpdateStyling(IsListItemSelected());
}

void UModioCommonModGalleryEntry::NativeUpdateStyling(bool bIsListItemSelected)
{
	if (!ModioStyle)
	{
		UE_LOG(ModioUI, Error, TEXT("Unable to update styling for mod gallery entry '%s': ModioStyle is invalid"), *GetName());
		return;
	}

	TSubclassOf<UModioCommonDynamicImageStyle> CurrentStateStyle = bIsListItemSelected
																			 ? ModioStyle.GetDefaultObject()->ModImageSelectedStyle
																			 : ModioStyle.GetDefaultObject()->ModImageNormalStyle;

	if (!CurrentStateStyle)
	{
		UE_LOG(ModioUI, Error, TEXT("Unable to update styling for mod gallery entry '%s': CurrentStateStyle is invalid"), *GetName());
		return;
	}

	if (ModImage)
	{
		ModImage->SetStyle(CurrentStateStyle);
	}
}
