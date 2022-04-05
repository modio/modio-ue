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
	friend struct FModioImageWrapper;

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