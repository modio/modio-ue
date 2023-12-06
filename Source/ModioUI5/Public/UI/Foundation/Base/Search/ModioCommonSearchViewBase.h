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

UCLASS(BlueprintType)
class MODIOUI5_API UModioSearchResultsParamsUI : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ModioFilterParamsUI")
	FModioModCategoryParams CurrentFilterParams;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ModioFilterParamsUI")
	EModioCommonSearchViewType SearchType;
};

/**
 * @brief Search View Base class that allows the user to search for mods
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI5_API UModioCommonSearchViewBase : public UModioCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnSetDataSource() override;
};
