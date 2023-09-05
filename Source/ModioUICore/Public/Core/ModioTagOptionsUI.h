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

#include "Algo/Transform.h"
#include "CoreMinimal.h"
#include "Types/ModioModTagOptions.h"
#include "UObject/NoExportTypes.h"

#include "ModioTagOptionsUI.generated.h"

/**
* Wrapper around FModioModTagInfo because UMG widgets expect UObjects for data sources or list items
**/
UCLASS(BlueprintType)
class MODIOUICORE_API UModioTagInfoUI : public UObject
{
	GENERATED_BODY()

public:
	/**
	* Stored property to the ModTagInfo to use in this class
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ModioTagInfoUI")
	FModioModTagInfo Underlying;

	/**
	* Stored property array to string pointers of the selected tags
	**/
	TArray<TSharedPtr<FString>> SelectedTagValues;
};

/**
 * Wrapper around a FModioModTagOptions because UMG widgets expect UObjects for data sources or list items
 */
UCLASS(BlueprintType)
class MODIOUICORE_API UModioTagOptionsUI : public UObject
{
	GENERATED_BODY()
protected:
	TArray<UModioTagInfoUI*> CachedUITagInfos;

public:
	/**
	* Stored property of the ModioTagOptions to use in this class
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ModioTagOptionsUI")
	FModioModTagOptions Underlying;

	/**
	* Retrieve the tag categories stored in this tag options instance
	* @return Array with its corresponding info tags 
	**/
	UFUNCTION(BlueprintCallable, Category = "ModioTagOptionsUI")
	TArray<class UModioTagInfoUI*> GetTagCategoriesForUI()
	{
		if (CachedUITagInfos.Num() != Underlying.GetRawList().Num())
		{
			CachedUITagInfos.Empty();
			Algo::Transform(Underlying.GetRawList(), CachedUITagInfos, [](const FModioModTagInfo& TagInfo) {
				UModioTagInfoUI* NewTagInfo = NewObject<UModioTagInfoUI>();
				NewTagInfo->Underlying = TagInfo;
				return NewTagInfo;
			});
		}
		return CachedUITagInfos;
	}
};
