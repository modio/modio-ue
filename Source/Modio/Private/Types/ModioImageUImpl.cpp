/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "Types/ModioImageWrapper.h"

#include "Async/Async.h"
#include "Engine/Engine.h"
#include "Engine/Texture2DDynamic.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Internal/ModioConvert.h"
#include "Misc/EngineVersionComparison.h"
#include "Misc/FileHelper.h"
#include "ModioImageCache.h"
#include "ModioSubsystem.h"
#include "Modules/ModuleManager.h"
#include "RenderingThread.h"
#include "TextureResource.h"

#if UE_VERSION_NEWER_THAN(5,2,0)
	#include "TextureResource.h"
	#define MODIO_UE_TEXTURE_RESOURCE_TYPE FTextureResource
#else 
	#define MODIO_UE_TEXTURE_RESOURCE_TYPE FTexture2DDynamicResource
#endif

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioImageWrapper)

#if !UE_SERVER
static TOptional<TArray<uint8>> GetImageData(TSharedPtr<IImageWrapper> ImageWrapper, ERGBFormat InFormat);
static ERGBFormat GetDesiredErgbFormat();

static void WriteRawToTexture_RenderThread(MODIO_UE_TEXTURE_RESOURCE_TYPE* TextureResource,
										   const TArray<uint8>& RawData,
										   bool bUseSRGB = true);
#endif

class UTexture2DDynamic* FModioImageWrapper::GetTexture() const
{
	UTexture2DDynamic* Texture = nullptr;
	if (UModioSubsystem* ModioSubsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Texture = ModioSubsystem->GetImageCache().GetImage(ImagePath);
	}

	return Texture;
}

TSharedPtr<FModioImageWrapper::FTextureCreationData> FModioImageWrapper::LoadTextureDataFromDisk(
	const FString& ImagePath)
{
#if !UE_SERVER
	IImageWrapperModule& ImageWrapperModule =
		FModuleManager::GetModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	TSharedPtr<IImageWrapper> ImageWrappers[] = {
		ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG),
		ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG),
	};

	for (auto ImageWrapper : ImageWrappers)
	{
		TArray<uint8> Data;
		FFileHelper::LoadFileToArray(Data, *ImagePath, FILEREAD_None);
		if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(Data.GetData(), Data.Num()))
		{
			if (TOptional<TArray<uint8>> RawData = GetImageData(ImageWrapper, GetDesiredErgbFormat()))
			{
				TSharedPtr<FTextureCreationData> data
					(new FTextureCreationData(RawData.GetValue(), ImageWrapper->GetWidth(), ImageWrapper->GetHeight()));
				return data;
			}
		}
	}
#endif
	return {};
}

EModioImageState FModioImageWrapper::GetState() const
{
	if (UModioSubsystem* ModioSubsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		return ModioSubsystem->GetImageCache().GetImageState(ImagePath);
	}
	return EModioImageState::OnDisc;
}

void FModioImageWrapper::LoadAsync(FOnLoadImageDelegateFast Callback) const
{
#if UE_SERVER
	Callback.ExecuteIfBound(nullptr);
#else
	// Check if it's already loaded, if so, don't load it again
	if (UTexture2DDynamic* Texture = GetTexture())
	{
		Callback.ExecuteIfBound(Texture);
	}
	else
	{
		if (UModioSubsystem* ModioSubsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
		{
			ModioSubsystem->GetImageCache().UpdateImageState(ImagePath, EModioImageState::LoadingIntoMemory);
		}
		// Create a background task to load the texture in
		AsyncTask(ENamedThreads::AnyNormalThreadNormalTask, [ImagePath = this->ImagePath, Callback]() {
			TSharedPtr<FTextureCreationData> TextureData = LoadTextureDataFromDisk(ImagePath);
			
			// NOTE: Creating UObjects outside of game thread will cause a crash when CancelAsyncLoading is called

			// Send back data to game thread
			AsyncTask(ENamedThreads::GameThread, [Callback, TextureData, ImagePath]() mutable {
				TArray<uint8> RawTextureData;
				UTexture2DDynamic* Texture = nullptr;

				// If we are not in the process of shutting down, create the texture
				if (!GExitPurge && !IsGarbageCollecting())
				{
					if (TextureData)
					{
						Texture = UTexture2DDynamic::Create(TextureData->Width, TextureData->Height);

						if (Texture)
						{
							// Ensure that the texture won't get garbage collected until the client has been able to grab a
							// reference to it as we are doing async calls
							Texture->AddToRoot();
							Texture->SRGB = true;
							RawTextureData = MoveTemp(TextureData->RawData);
						}
					}
				}
				if (UModioSubsystem* ModioSubsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
				{
					ModioSubsystem->GetImageCache().UpdateImageState(
						ImagePath, Texture ? EModioImageState::InMemory : EModioImageState::Corrupted, Texture);
				}

				if (Texture)
				{
					// Now the client has had a chance to grab a reference to the texture, so we can now remove it from
					// the
					// root set
					Texture->RemoveFromRoot();

					Texture->UpdateResource();
	#if UE_VERSION_NEWER_THAN(5, 0, 0)
					MODIO_UE_TEXTURE_RESOURCE_TYPE* TextureResource =
						static_cast<MODIO_UE_TEXTURE_RESOURCE_TYPE*>(Texture->GetResource());
	#else
					MODIO_UE_TEXTURE_RESOURCE_TYPE* TextureResource =
						static_cast<MODIO_UE_TEXTURE_RESOURCE_TYPE*>(Texture->Resource);
	#endif
					ENQUEUE_RENDER_COMMAND(FWriteRawDataToTexture)
					([TextureResource,
					  RawDataRenderingThread = MoveTemp(RawTextureData)](FRHICommandListImmediate& RHICmdList) {
						WriteRawToTexture_RenderThread(TextureResource, RawDataRenderingThread);
					});
				}

				Callback.ExecuteIfBound(Texture);
				
			});
		});
	}
#endif
}

#if !UE_SERVER
void WriteRawToTexture_RenderThread(MODIO_UE_TEXTURE_RESOURCE_TYPE* TextureResource, const TArray<uint8>& RawData,
									bool bUseSRGB)
{
	check(IsInRenderingThread());

	// Using auto keyword as the type is different after 4.25 but is used in the same manner
	auto TextureRHI = TextureResource->GetTexture2DRHI();

	int32 Width = TextureRHI->GetSizeX();
	int32 Height = TextureRHI->GetSizeY();

	uint32 DestStride = 0;
	uint8* DestData =
		reinterpret_cast<uint8*>(RHILockTexture2D(TextureRHI, 0, RLM_WriteOnly, DestStride, false, false));

	for (int32 y = 0; y < Height; y++)
	{
		uint8* DestPtr = &DestData[(Height - 1 - y) * DestStride];

		const FColor* SrcPtr = &((FColor*) (RawData.GetData()))[(Height - 1 - y) * Width];
		for (int32 x = 0; x < Width; x++)
		{
			*DestPtr++ = SrcPtr->B;
			*DestPtr++ = SrcPtr->G;
			*DestPtr++ = SrcPtr->R;
			*DestPtr++ = SrcPtr->A;
			SrcPtr++;
		}
	}

	RHIUnlockTexture2D(TextureRHI, 0, false, false);
}

ERGBFormat GetDesiredErgbFormat()
{
	#if ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 22 && ENGINE_MINOR_VERSION <= 23
	return IsHTML5Platform() ? ERGBFormat::RGBA : ERGBFormat::BGRA;
	#elif ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION == 24
	return (GShaderPlatformForFeatureLevel[GMaxRHIFeatureLevel] == SP_OPENGL_ES2_WEBGL) ? ERGBFormat::RGBA
																						: ERGBFormat::BGRA;
	#else // 4.25 ->
	return ERGBFormat::BGRA;
	#endif
}

TOptional<TArray<uint8>> GetImageData(TSharedPtr<IImageWrapper> ImageWrapper, ERGBFormat InFormat)
{
	TArray<uint8> RawData;
	#if ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 22 && ENGINE_MINOR_VERSION <= 24
	const TArray<uint8>* OriginalRawData = nullptr;
	if (ImageWrapper->GetRaw(InFormat, 8, OriginalRawData))
	{
		// Copy out the data
		RawData = *OriginalRawData;
		return RawData;
	}
	#else // 4.25 ->
	TArray<uint8> OriginalRawData;
	if (ImageWrapper->GetRaw(InFormat, 8, OriginalRawData))
	{
		RawData = MoveTemp(OriginalRawData);
		return RawData;
	}
	#endif

	return {};
}

#endif
