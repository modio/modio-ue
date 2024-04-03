/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "CoreMinimal.h"
#include "Core/ModioFilterParamsUI.h"
#include "Types/ModioFilterParams.h"
#include "UI/Foundation/Base/ModioCommonActivatableWidget.h"
#include "ModioCommonSearchViewBase.generated.h"

enum class EModioCommonSearchViewType : uint8;

/**
 * Parameters for the search view
 */
UCLASS(BlueprintType)
class MODIOUI_API UModioSearchResultsParamsUI : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Filter parameters to use when searching for mods
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ModioFilterParamsUI")
	FModioModCategoryParams FilterParams;

	/**
	 * Default filter parameters to use when resetting the search
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ModioFilterParamsUI")
	FModioModCategoryParams DefaultFilterParams;

	/**
	 * The type of search to perform
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ModioFilterParamsUI")
	EModioCommonSearchViewType SearchType;
};

/**
 * Search View Base class that allows the user to search for mods
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonSearchViewBase : public UModioCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	//~ Begin UModioCommonActivatableWidget Interface
	virtual void NativeOnSetDataSource() override;
	//~ End UModioCommonActivatableWidget Interface
};
