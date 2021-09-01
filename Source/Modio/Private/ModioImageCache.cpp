/* 
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *  
 *  This file is part of the mod.io UE4 Plugin.
 *  
 *  Distributed under the MIT License. (See accompanying file LICENSE or 
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *   
 */

#include "ModioImageCache.h"
#include "Engine/Texture2DDynamic.h"

bool FModioImageCache::IsStaleState(const FImageState& State) const
{
	return !State.Texture.IsValid(false) &&
		   (State.State == EModioImageState::InMemory || State.State == EModioImageState::OnDisc);
}

class UTexture2DDynamic* FModioImageCache::GetImage(const FString& ImagePath)
{
	FImageState* State = ImageCache.Find(ImagePath);
	if (State == nullptr)
	{
		return nullptr;
	}

	// Copy the texture before potentially removing it, so that we don't invalidate the pointer
	UTexture2DDynamic* Texture = State->Texture.Get(false);
	if (IsStaleState(*State))
	{
		// Ensure that we clean out stale records when they are queried
		ImageCache.Remove(ImagePath);
	}
	return Texture;
}

EModioImageState FModioImageCache::GetImageState(const FString& ImagePath)
{
	FImageState* State = ImageCache.Find(ImagePath);
	if (State == nullptr)
	{
		return EModioImageState::OnDisc;
	}

	// Copy the state before potentially removing it, so that we don't invalidate the pointer
	const EModioImageState TextureState = State->State;
	if (IsStaleState(*State))
	{
		// Ensure that we clean out stale records when they are queried
		ImageCache.Remove(ImagePath);
	}
	return TextureState;
}

void FModioImageCache::UpdateImageState(const FString& ImagePath, EModioImageState NewState,
										class UTexture2DDynamic* Image /* = nullptr */)
{
	ImageCache.FindOrAdd(ImagePath) = {Image, NewState };
}
