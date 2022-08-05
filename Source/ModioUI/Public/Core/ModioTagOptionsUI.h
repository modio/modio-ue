// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Algo/Transform.h"
#include "CoreMinimal.h"
#include "Types/ModioModTagOptions.h"
#include "UObject/NoExportTypes.h"

#include "ModioTagOptionsUI.generated.h"

/// @brief Wrapper around FModioModTagInfo because UMG widgets expect UObjects for data sources or list items
UCLASS(BlueprintType)
class MODIOUI_API UModioTagInfoUI : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="ModioTagInfoUI")
	FModioModTagInfo Underlying;

	TArray<TSharedPtr<FString>> SelectedTagValues;
};

/**
 * Wrapper around a FModioModTagOptions because UMG widgets expect UObjects for data sources or list items
 */
UCLASS(BlueprintType)
class MODIOUI_API UModioTagOptionsUI : public UObject
{
	GENERATED_BODY()
protected:
	TArray<UModioTagInfoUI*> CachedUITagInfos;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="ModioTagOptionsUI")
	FModioModTagOptions Underlying;

	UFUNCTION(BlueprintCallable, Category="ModioTagOptionsUI")
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
