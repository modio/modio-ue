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

class UModioCommonTextStyle;
class UModioCommonFilteringView;
class UModioCommonBorderStyle;
class UModioCommonButtonStyle;
class UModioCommonEditableTextBoxStyle;

/**
 * The style of the Search Tab View within the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonSearchTabViewStyle : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style|Background")
	TSubclassOf<UModioCommonBorderStyle> InternalBackgroundBorderStyle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style|Background")
	TSubclassOf<UModioCommonBorderStyle> OverlayBackgroundBorderStyle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style|Background")
	TSubclassOf<UModioCommonTextStyle> SearchTabTitleTextStyle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style|Text")
	TSubclassOf<UModioCommonEditableTextBoxStyle> SearchTextBoxStyle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style|Button")
	TSubclassOf<UModioCommonButtonStyle> CloseButtonStyle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style|Button")
	TSubclassOf<UModioCommonButtonStyle> ResetButtonStyle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style|Button")
	TSubclassOf<UModioCommonButtonStyle> SearchButtonStyle;
};
