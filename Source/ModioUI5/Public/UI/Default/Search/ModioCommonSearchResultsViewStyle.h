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
 * @brief The style of the Search Results View within the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI5_API UModioCommonSearchResultsViewStyle : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style|View")
	TSubclassOf<UModioCommonFilteredModListViewStyle> FilteredModListViewStyle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style|Text")
	TSubclassOf<UModioCommonTextStyle> KeywordsLabelTextStyle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style|Text")
	TSubclassOf<UModioCommonTextStyle> KeywordsDetailsTextStyle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style|Text")
	TSubclassOf<UModioCommonTextStyle> CategoriesLabelTextStyle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style|Button")
	TSubclassOf<UModioCommonButtonStyle> EditSearchButtonStyle;
};
