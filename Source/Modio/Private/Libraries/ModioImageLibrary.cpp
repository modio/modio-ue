/* 
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *  
 *  This file is part of the mod.io UE4 Plugin.
 *  
 *  Distributed under the MIT License. (See accompanying file LICENSE or 
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *   
 */

#include "Libraries/ModioImageLibrary.h"
#include "Engine/Texture.h"
#include "Internal/ModioPrivateDefines.h"

class UTexture2DDynamic* UModioImageLibrary::GetTexture(const FModioImageWrapper& Image)
{
	return Image.GetTexture();
}

void UModioImageLibrary::LoadAsync(const FModioImageWrapper& Image, FOnLoadImageDelegate OnImageLoaded)
{
	Image.LoadAsync(FOnLoadImageDelegateFast::CreateLambda(
		[OnImageLoaded](UTexture2DDynamic* Texture) { OnImageLoaded.ExecuteIfBound(Texture); }));
}

EModioImageState UModioImageLibrary::GetState(const FModioImageWrapper& Image)
{
	return Image.GetState();
}

MODIO_BEGIN_CONVERT_SWITCHES
FVector2D UModioImageLibrary::GetLogoSize(UTexture* Logo, EModioLogoSize LogoSize)
{
	switch (LogoSize)
	{
		case EModioLogoSize::Original:
			return IsValid(Logo) ? FVector2D(Logo->GetSurfaceWidth(), Logo->GetSurfaceHeight())
								 : FVector2D(EForceInit::ForceInitToZero);
		case EModioLogoSize::Thumb320:
			return FVector2D(320, 180);
		case EModioLogoSize::Thumb640:
			return FVector2D(640, 360);
		case EModioLogoSize::Thumb1280:
			return FVector2D(1280, 720);
	}

	return FVector2D(EForceInit::ForceInitToZero);
}

FVector2D UModioImageLibrary::GetAvatarSize(UTexture* Avatar, EModioAvatarSize AvatarSize)
{
	switch (AvatarSize)
	{
		case EModioAvatarSize::Original:
			return IsValid(Avatar) ? FVector2D(Avatar->GetSurfaceWidth(), Avatar->GetSurfaceHeight())
								   : FVector2D(EForceInit::ForceInitToZero);
		case EModioAvatarSize::Thumb50:
			return FVector2D(50, 50);
		case EModioAvatarSize::Thumb100:
			return FVector2D(100, 100);
	}

	return FVector2D(EForceInit::ForceInitToZero);
}

FVector2D UModioImageLibrary::GetGallerySize(UTexture* GalleryImage, EModioGallerySize GallerySize)
{
	switch (GallerySize)
	{
		case EModioGallerySize::Original:
			return IsValid(GalleryImage) ? FVector2D(GalleryImage->GetSurfaceWidth(), GalleryImage->GetSurfaceHeight())
										 : FVector2D(EForceInit::ForceInitToZero);
		case EModioGallerySize::Thumb320:
			return FVector2D(320, 180);
	}

	return FVector2D(EForceInit::ForceInitToZero);
}
MODIO_END_CONVERT_SWITCHES
