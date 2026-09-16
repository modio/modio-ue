/*
 *  Copyright (C) 2026 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "Containers/UnrealString.h"
#include "Types/ModioFilterParams.h"

#include "ModioPlacement.generated.h"

/*
 * @docpublic
 * @brief Type of the placement
 */
UENUM(BlueprintType)
enum class EModioPlacementType : uint8
{
	Filters = 0 /** (default) */
};

/*
 * @docpublic
 * @brief Size of the placement to display
 */
UENUM(BlueprintType)
enum class EModioPlacementSize : uint8
{
	Small = 0, /** (default) */
	Medium = 1,
	Large = 2
};

/**
 * @docpublic
 * @brief Placements of a game to represent dynamic mod view
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioPlacement
{
	GENERATED_BODY()

	/**
	 * @docpublic
	 * @brief Unique placement id
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "mod.io|Placements")
	int64 ID = -1;

	/**
	 * @docpublic
	 * @brief Placement type
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "mod.io|Placements")
	EModioPlacementType Type = EModioPlacementType::Filters;

	/**
	 * @docpublic
	 * @brief Placement name
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "mod.io|Placements")
	FString Name;

	/**
	 * @docpublic
	 * @brief Placement name localized
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "mod.io|Placements")
	FString NameLocalized;

	/**
	 * @docpublic
	 * @brief Structured filter describing the mods this placement should display
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "mod.io|Placements")
	FModioFilterParams Filters;

	/**
	 * @docpublic
	 * @brief Placement's display position
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "mod.io|Placements")
	int64 DisplayPosition = -1;

	/**
	 * @docpublic
	 * @brief Total number in the placement to show
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "mod.io|Placements")
	int64 Total = 100;

	/**
	 * @docpublic
	 * @brief Placement's display size
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "mod.io|Placements")
	EModioPlacementSize Size = EModioPlacementSize::Medium;

	/**
	 * @docpublic
	 * @brief Placement's enabled status
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "mod.io|Placements")
	bool bEnabled = true;

	
};