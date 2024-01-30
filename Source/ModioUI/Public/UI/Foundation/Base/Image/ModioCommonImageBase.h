/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Types/ModioImageWrapper.h"
#include "Types/ModioCommonTypes.h"

#include "ModioCommonImageBase.generated.h"

class UCommonLazyImage;
struct FSlateBrush;

/**
 * Image Base class that handles downloading and displaying images from Mod.io, as well as directly
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonImageBase : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	/** Loads and sets the texture to the avatar image */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Image")
	void LoadImageFromAvatar(FModioModID ModID, EModioAvatarSize AvatarSize);

	/** Loads and sets the texture to the logo image */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Image")
	void LoadImageFromLogo(FModioModID ModID, EModioLogoSize LogoSize);

	/** Loads and sets the texture to the image at a specified index in the gallery */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Image")
	void LoadImageFromGallery(FModioModID ModID, EModioGallerySize GallerySize, int32 Index);

	/** Loads and sets the texture to the Image on disk */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Image")
	void LoadImageFromFileAsync(FModioImageWrapper ImageLoader);

	/** Executed when the image has finished loading, with the result of the load */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Image")
	void SetImageTexture(UTexture2DDynamic* Texture);

	/** Executed when the image is about to start loading (e.g. to show some kind of loading indicator) */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Image")
	void OnStartLoadNewImage();

	/** Executed when the image has failed to load (e.g. to show some kind of error indicator) */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Image")
	void OnLoadNewImageError();

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnImageLoadedStaticDelegate, bool /**bSuccess*/);
	/** Called when the image has finished loading */
	FOnImageLoadedStaticDelegate OnImageLoaded;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnImageLoadedDynamicDelegate, bool, bSuccess);
	/** Called when the image has finished loading. Suitable for use in Blueprints */
	UPROPERTY(BlueprintAssignable, Category = "Mod.io Common UI|Image")
	FOnImageLoadedDynamicDelegate OnImageLoadedDynamic;

protected:
	/** Whether the load has failed or not */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Mod.io Common UI|Image")
	bool bLoadFailed = false;

	/** Whether the image is currently loading or not */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Mod.io Common UI|Image")
	bool bIsLoading = false;
};