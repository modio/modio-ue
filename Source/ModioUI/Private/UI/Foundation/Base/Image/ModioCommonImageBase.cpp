/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Foundation/Base/Image/ModioCommonImageBase.h"

#include "Types/ModioImageWrapper.h"
#include "ModioSubsystem.h"
#include "Engine/Texture2DDynamic.h"
#include "Types/ModioErrorCode.h"
#include "Types/ModioCommonTypes.h"
#include "Engine/Engine.h"

void UModioCommonImageBase::LoadImageFromAvatar(FModioModID ModID, EModioAvatarSize AvatarSize) 
{
	OnStartLoadNewImage();
	if (UModioSubsystem* ModioSubsystem = GEngine->GetEngineSubsystem<UModioSubsystem>()) 
	{
		ModioSubsystem->GetModMediaAsync(ModID, AvatarSize, FOnGetMediaDelegateFast::CreateLambda([this](FModioErrorCode ErrorCode, TOptional<FModioImageWrapper> ModioImageWrapper)
		{
			if (ModioImageWrapper.IsSet())
			{
				LoadImageFromFileAsync(ModioImageWrapper.GetValue());
			}
		}));
	}
}

void UModioCommonImageBase::LoadImageFromLogo(FModioModID ModID, EModioLogoSize LogoSize) 
{
	OnStartLoadNewImage();
	if (UModioSubsystem* ModioSubsystem = GEngine->GetEngineSubsystem<UModioSubsystem>()) 
	{
		ModioSubsystem->GetModMediaAsync(ModID, LogoSize, FOnGetMediaDelegateFast::CreateWeakLambda(this, [this](FModioErrorCode ErrorCode, TOptional<FModioImageWrapper> ModioImageWrapper)
		{
			if (ModioImageWrapper.IsSet())
			{
				LoadImageFromFileAsync(ModioImageWrapper.GetValue());
			}
			else
			{
				OnLoadNewImageError();
			}
		}));
	}
}

void UModioCommonImageBase::LoadImageFromGallery(FModioModID ModID, EModioGallerySize GallerySize, int32 Index) 
{
	OnStartLoadNewImage();
	if (UModioSubsystem* ModioSubsystem = GEngine->GetEngineSubsystem<UModioSubsystem>()) 
	{
		ModioSubsystem->GetModMediaAsync(ModID, GallerySize, Index, FOnGetMediaDelegateFast::CreateWeakLambda(this, [this](FModioErrorCode ErrorCode, TOptional<FModioImageWrapper> ModioImageWrapper)
		{
			if (ModioImageWrapper.IsSet())
			{
				LoadImageFromFileAsync(ModioImageWrapper.GetValue());
			}
			else
			{
				OnLoadNewImageError();
			}
		}));
	}
}

void UModioCommonImageBase::LoadImageFromFileAsync(FModioImageWrapper ImageLoader)
{
	OnStartLoadNewImage();
	ImageLoader.LoadAsync(FOnLoadImageDelegateFast::CreateWeakLambda(this, [this](UTexture2DDynamic* Texture) 
	{
		if (Texture)
		{
			SetImageTexture(Texture);
		}
		else
		{
			OnLoadNewImageError();
		}
	}));
}

void UModioCommonImageBase::SetImageTexture_Implementation(UTexture2DDynamic* Texture)
{
	bLoadFailed = false;
	bIsLoading = false;
	OnImageLoaded.Broadcast(true);
	OnImageLoadedDynamic.Broadcast(true);
}

void UModioCommonImageBase::OnStartLoadNewImage_Implementation()
{
	bLoadFailed = false;
	bIsLoading = true;
	SynchronizeProperties();
}

void UModioCommonImageBase::OnLoadNewImageError_Implementation()
{
	bLoadFailed = true;
	bIsLoading = false;
	OnImageLoaded.Broadcast(false);
	OnImageLoadedDynamic.Broadcast(false);
	SynchronizeProperties();
}
