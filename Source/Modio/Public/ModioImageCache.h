﻿#pragma once

#include "UObject/WeakObjectPtr.h"
#include "Types/ModioImageState.h"

/**
 * Responsible for keeping track of the state of all the images that we have loaded/unloaded into
 * memory
 */
struct FModioImageCache
{
	/** */
	class UTexture2DDynamic* GetImage(const FString& ImagePath);
	EModioImageState GetImageState(const FString& ImagePath);
private:	
	friend struct FModioImage;

	void UpdateImageState(const FString& ImagePath, EModioImageState NewState, class UTexture2DDynamic* Image = nullptr);
	
	struct FImageState
	{
		TWeakObjectPtr<class UTexture2DDynamic> Texture = nullptr;
		EModioImageState State = EModioImageState::OnDisc;
	};

	bool IsStaleState(const FImageState& State) const;

	/** Map of all states of all the available images */
	TMap<FString, FImageState> ImageCache;
};