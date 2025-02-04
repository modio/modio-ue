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
 * @docpublic
 * @brief Strong type struct to wrap image data returned by the mod.io API
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioImageWrapper
{
	GENERATED_BODY()

	/**
	 * @docpublic
	 * @brief Get the texture if it has been loaded by any FModioImage instance
	 * @return The texture object if it has been loaded
	 */
	class UTexture2DDynamic* GetTexture() const;

	/**
	 * @docpublic
	 * @brief Get the current state of the image
	 * @return The current state of the image
	 */
	EModioImageState GetState() const;

	/**
	 * @docpublic
	 * @brief Load the texture into memory asynchronously. If it's already in memory, it will return immediately
	 * @param Callback A delegate to be called once the image is loaded
	 */
	void LoadAsync(FOnLoadImageDelegateFast Callback) const;

	/**
	 * @docpublic
	 * @brief The path on disk to the image
	 */
	UPROPERTY(BlueprintReadOnly, Category = "Media")
	FString ImagePath;

private:
	/**
	 * @brief Data required to create the texture
	 */
	struct FTextureCreationData
	{
		TArray<uint8> RawData;
		int32 Width;
		int32 Height;

		/**
		 * @brief Constructor for the texture creation data
		 * @param InRawData Raw image data
		 * @param InWidth Width of the image
		 * @param InHeight Height of the image
		 */
		FTextureCreationData(TArray<uint8> InRawData, int32 InWidth, int32 InHeight)
		{
			RawData = InRawData;
			Width = InWidth;
			Height = InHeight;
		}
	};

	/**
	 * @docpublic
	 * @brief Internal loader that will load the texture synchronously
	 * @param ImagePath The path of the image on disk
	 * @return A shared pointer to the texture creation data
	 */
	static TSharedPtr<FTextureCreationData> LoadTextureDataFromDisk(const FString& ImagePath);
};

/**
 * @docpublic
 * @brief Strong type struct to wrap Image data as an optional value
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioOptionalImage
{
	GENERATED_BODY()

	/**
	 * @docpublic
	 * @brief Optional value of a Modio image wrapper
	 */
	TOptional<FModioImageWrapper> Internal;
};