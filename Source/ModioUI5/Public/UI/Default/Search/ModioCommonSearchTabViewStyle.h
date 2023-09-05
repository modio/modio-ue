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
#include "ModioCommonSearchTabViewStyle.generated.h"

class UModioCommonFilteringView;
class UModioCommonSortingView;
class UModioCommonTabButtonBase;
class UModioCommonTabButtonStyle;
class UModioCommonBorderStyle;
class UModioCommonButtonStyle;
class UModioCommonEditableTextBoxStyle;

/**
 * @brief The style of the Search Tab View within the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI5_API UModioCommonSearchTabViewStyle : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class|View")
	TSubclassOf<UModioCommonSortingView> SortingViewClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class|View")
	TSubclassOf<UModioCommonFilteringView> FilteringViewClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class|Tab")
	TSubclassOf<UModioCommonTabButtonBase> SortingTabButtonClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class|Tab")
	TSubclassOf<UModioCommonTabButtonBase> FilteringTabButtonClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style|Tab")
	TSubclassOf<UModioCommonTabButtonStyle> SortingTabButtonStyle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style|Tab")
	TSubclassOf<UModioCommonTabButtonStyle> FilteringTabButtonStyle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style|Background")
	TSubclassOf<UModioCommonBorderStyle> InternalBackgroundBorderStyle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style|Background")
	TSubclassOf<UModioCommonBorderStyle> OverlayBackgroundBorderStyle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style|Text")
	TSubclassOf<UModioCommonEditableTextBoxStyle> SearchTextBoxStyle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style|Button")
	TSubclassOf<UModioCommonButtonStyle> CloseButtonStyle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style|Button")
	TSubclassOf<UModioCommonButtonStyle> ClearAllButtonStyle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style|Button")
	TSubclassOf<UModioCommonButtonStyle> SearchButtonStyle;
};
