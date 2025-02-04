/* 
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *  
 *  This file is part of the mod.io UE Plugin.
 *  
 *  Distributed under the MIT License. (See accompanying file LICENSE or 
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *   
 */

#pragma once
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Types/ModioCommonTypes.h"
#include "Types/ModioImageState.h"
#include "Types/ModioImageWrapper.h"

#include "ModioImageLibrary.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnLoadImageDelegate, class UTexture2DDynamic*, Texture);

/**
 * @docpublic
 * @brief Utility functions for loading and managing images
 */
UCLASS()
class MODIO_API UModioImageLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * @docpublic
	 * @brief Get the texture if it has been loaded by any FModioImage instance
	 * @param Image The image to get the texture from
	 * @return The texture if it has been loaded, otherwise nullptr
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Image")
	static class UTexture2DDynamic* GetTexture(const FModioImageWrapper& Image);

	/**
	 * @docpublic
	 * @brief Load the texture into memory asynchronously. If it's already in memory, it will be returned immediately.
	 * @param Image The image to load
	 * @param OnImageLoaded Called when the image has been loaded
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|Image")
	static void LoadAsync(const FModioImageWrapper& Image, FOnLoadImageDelegate OnImageLoaded);

	/**
	 * @docpublic
	 * @brief Get the current state of the image
	 * @param Image The image to get the state of
	 * @return The state of the image
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Image")
	static EModioImageState GetState(const FModioImageWrapper& Image);

	/**
	 * @docpublic
	 * @brief If the logo size is EModioLogoSize::Original, the original logo size is returned. 
	 * Otherwise, the thumbnail size of the logo is returned.
	 * @param Logo If null and `EModioLogoSize::Original` is passed, (0, 0) is returned.
	 * @param LogoSize The size of the logo to return
	 *
	 * @return Dimensions of the logo when displayed in a 1:1 pixel ratio
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Image")
	static FVector2D GetLogoSize(class UTexture* Logo, EModioLogoSize LogoSize);

	/**
	 * @docpublic
	 * @brief If the avatar size is EModioAvatarSize::Original, the original avatar size is returned. 
	 * Otherwise, the thumbnail size of the avatar is returned.
	 * @param Avatar If null and EModioAvatarSize::Original is passed, (0, 0) is returned.
	 * @param AvatarSize The size of the avatar to return
	 *
	 * @return Dimensions of the avatar when displayed in a 1:1 pixel ratio
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Image")
	static FVector2D GetAvatarSize(class UTexture* Avatar, EModioAvatarSize AvatarSize);

	/**
	 * @docpublic
	 * @brief If the gallery image size is EModioGallerySize::Original, the original gallery image size is returned. 
	 * Otherwise, the thumbnail size of the gallery image is returned.
	 * @param GalleryImage If null and EModioGallerySize::Original is passed, (0, 0) is returned.
	 * @param GallerySize The size of the gallery image to return
	 *
	 * @return Dimensions of the gallery image when displayed in a 1:1 pixel ratio
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Image")
	static FVector2D GetGallerySize(class UTexture* GalleryImage, EModioGallerySize GallerySize);
};
