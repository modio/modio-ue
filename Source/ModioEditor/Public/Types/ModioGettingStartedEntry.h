/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#pragma once

#include "CoreMinimal.h"

#include "Objects/ModioStaticExecutionBase.h"
#include "Engine/Texture2D.h"

#include "ModioGettingStartedEntry.generated.h"

USTRUCT(BlueprintType)
struct MODIOEDITOR_API FModioGettingStartedEntry
{
	GENERATED_BODY()

	/* If false, this entry will not be displayed in the Getting Started Widget. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modio Getting Started Entry")
	bool bEnabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modio Getting Started Entry")
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modio Getting Started Entry")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modio Getting Started Entry")
	TSoftObjectPtr<UTexture2D> Icon;

	/* The class of action to be executed upon the button being clicked.
	 * This should be a child of ModioStaticExecutionBase that overrides ExecuteAction
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modio Getting Started Entry")
	TSubclassOf<UModioStaticExecutionBase> Action;

	/* Arguments to be passed to the Action class. Not all actions require arguments. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Modio Getting Started Entry")
	FString Arguments;

	FORCEINLINE bool operator==(const FModioGettingStartedEntry& Other) const
	{
		return Name.EqualTo(Other.Name);
	}

	FORCEINLINE bool operator!=(const FModioGettingStartedEntry& Other) const
	{
		return !Name.EqualTo(Other.Name);
	}

	FORCEINLINE friend uint32 GetTypeHash(const FModioGettingStartedEntry& Entry)
	{
		return HashCombine(GetTypeHash(Entry.Name.ToString()), GetTypeHash(Entry.bEnabled));
	}
};
