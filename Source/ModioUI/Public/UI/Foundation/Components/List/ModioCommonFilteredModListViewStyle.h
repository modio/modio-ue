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
#include "ModioCommonFilteredModListViewStyle.generated.h"

class UModioCommonTextStyle;
class UModioCommonButtonStyle;
class UModioCommonListViewStyle;

/**
 * The style of the Filtered Mod List View within the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonFilteredModListViewStyle : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	TSubclassOf<UModioCommonListViewStyle> ModListStyle;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	TSubclassOf<UModioCommonButtonStyle> PreviousPageButtonStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	TSubclassOf<UModioCommonButtonStyle> NextPageButtonStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	TSubclassOf<UModioCommonTextStyle> CurrentPageTextBlockStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	TSubclassOf<UModioCommonTextStyle> TotalPagesTextBlockStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	TSubclassOf<UModioCommonTextStyle> TotalModsTextBlockStyle;
};
