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

#include "Containers/UnrealString.h"
#include "Types/ModioImageState.h"

#include "ModioImageWrapper.generated.h"


DECLARE_DELEGATE_OneParam(FOnLoadImageDelegateFast, class UTexture2DDynamic*);

/** @brief wrapper for images that's returned by the api */
USTRUCT(BlueprintType)
struct MODIO_API FModioImageWrapper
{
	GENERATED_BODY()

	/** Get the texture if if has been loaded by any FModioImage instance */
	class UTexture2DDynamic* GetTexture() const;

	/** Get the current state of the image */
	EModioImageState GetState() const;

	/** Load the texture into memory async, if it's already in memory, then we will return immediately */
	void LoadAsync(FOnLoadImageDelegateFast Callback) const;

	/** The path on disc to the image */
	UPROPERTY(BlueprintReadOnly, Category = "Media")
	FString ImagePath;
private:
	// Data required to create the texture
	struct FTextureCreationData
	{
		TArray<uint8> RawData;
		int32 Width;
		int32 Height;
	};

	/** Internal loader that will load the texture sync */
	static TOptional<FTextureCreationData> LoadTextureDataFromDisk(const FString& ImagePath);
};

USTRUCT(BlueprintType)
struct MODIO_API FModioOptionalImage
{
	GENERATED_BODY()

	TOptional<FModioImageWrapper> Internal;
};
