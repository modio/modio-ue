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
#include "UObject/Object.h"
#include "Templates/SubclassOf.h"
#include "ModioCommonSearchResultsViewStyle.generated.h"

class UModioCommonButtonStyle;
class UModioCommonTextStyle;
class UModioCommonFilteredModListViewStyle;

/**
 * The style of the Search Results View within the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonSearchResultsViewStyle : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mod.io Common UI|Style")
	TSubclassOf<UModioCommonFilteredModListViewStyle> FilteredModListViewStyle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mod.io Common UI|Style")
	TSubclassOf<UModioCommonTextStyle> KeywordsLabelTextStyle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mod.io Common UI|Style")
	TSubclassOf<UModioCommonTextStyle> KeywordsDetailsTextStyle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mod.io Common UI|Style")
	TSubclassOf<UModioCommonTextStyle> CategoriesLabelTextStyle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mod.io Common UI|Style")
	TSubclassOf<UModioCommonButtonStyle> EditSearchButtonStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	TSubclassOf<UModioCommonTextStyle> FilterCounterTextStyle;
};
