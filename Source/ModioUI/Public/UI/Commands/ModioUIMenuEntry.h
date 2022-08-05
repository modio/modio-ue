// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Misc/Crc.h"
#include "UObject/NoExportTypes.h"

#include "ModioUIMenuEntry.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct MODIOUI_API FModioUIMenuEntry
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widget")
	FText MenuEntryLabel;

	bool operator==(const FModioUIMenuEntry& Other) const
	{
		return MenuEntryLabel.EqualTo(Other.MenuEntryLabel);
	}
};

uint32 GetTypeHash(const FModioUIMenuEntry& Object);