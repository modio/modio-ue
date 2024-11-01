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

#include "Containers/UnrealString.h"
#include "Types/ModioImageState.h"

#include "ModioImageWrapper.generated.h"


DECLARE_DELEGATE_OneParam(FOnLoadImageDelegateFast, class UTexture2DDynamic*);

/** 
* Strong type struct to wrap image data returned by the mod.io API
*/
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

		FTextureCreationData(TArray<uint8> InRawData, int32 InWidth, int32 InHeight)
		{
			RawData = InRawData;
			Width = InWidth;
			Height = InHeight;
		}
	};

	/** Internal loader that will load the texture sync */
	static TSharedPtr<FTextureCreationData> LoadTextureDataFromDisk(const FString& ImagePath);
};

/**
* Strong type struct to wrap Image data as an optional value
**/
USTRUCT(BlueprintType)
struct MODIO_API FModioOptionalImage
{
	GENERATED_BODY()

	/**
	* Optional value of a Modio image wrapper
	**/
	TOptional<FModioImageWrapper> Internal;
};
