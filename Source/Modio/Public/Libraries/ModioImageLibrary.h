/* 
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *  
 *  This file is part of the mod.io UE4 Plugin.
 *  
 *  Distributed under the MIT License. (See accompanying file LICENSE or 
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *   
 */

#pragma once
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Types/ModioCommonTypes.h"
#include "Types/ModioImageWrapper.h"
#include "Types/ModioImageState.h"

#include "ModioImageLibrary.generated.h"


DECLARE_DYNAMIC_DELEGATE_OneParam(FOnLoadImageDelegate, class UTexture2DDynamic*, Texture);

UCLASS()
class MODIO_API UModioImageLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/** Get the texture if if has been loaded by any FModioImage instance */
	UFUNCTION(BlueprintPure, Category = "mod.io|Image")
	static class UTexture2DDynamic* GetTexture(const FModioImageWrapper& Image);

	/** Load the texture into memory async, if it's already in memory, then we will return immediately */
	UFUNCTION(BlueprintCallable, Category = "mod.io|Image")
	static void LoadAsync(const FModioImageWrapper& Image, FOnLoadImageDelegate OnImageLoaded);

	/** Get the current state of the image */
	UFUNCTION(BlueprintPure, Category = "mod.io|Image")
	static EModioImageState GetState(const FModioImageWrapper& Image);

	/**
	 * If a logo size is EModioLogoSize::Original, then the size of the Logo returned,
	 * else, the thumbnail size a logo is returned
	 * @param Logo If null and EModioLogoSize::Original is passed, then (0, 0) is returned
	 * @param LogoSize The size of the logo we want to return
	 *
	 * @return Dimensions of the logo if displayed in a 1:1 pixel ratio
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Image")
	static FVector2D GetLogoSize(class UTexture* Logo, EModioLogoSize LogoSize);
	
	/**
	 * If a avatar size is EModioAvatarSize::Original, then the size of the Logo returned,
	 * else, the thumbnail size a avatar is returned
	 * @param Avatar If null and EModioAvatarSize::Original is passed, then (0, 0) is returned
	 * @param AvatarSize The size of the avatar we want to return
	 *
	 * @return Dimensions of the avatar if displayed in a 1:1 pixel ratio
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Image")
	static FVector2D GetAvatarSize(class UTexture* Avatar, EModioAvatarSize AvatarSize);

	/**
	 * If a gallery image size is EModioGallerySize::Original, then the size of the gallery image returned,
	 * else, the thumbnail size a gallery image is returned
	 * @param GalleryImage If null and EModioGallerySize::Original is passed, then (0, 0) is returned
	 * @param GallerySize The size of the gallery image we want to return
	 *
	 * @return Dimensions of the gallery image if displayed in a 1:1 pixel ratio
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Image")
	static FVector2D GetGallerySize(class UTexture* GalleryImage, EModioGallerySize GallerySize);
};
