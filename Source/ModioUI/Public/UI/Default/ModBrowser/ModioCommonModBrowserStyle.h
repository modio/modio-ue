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
#include "UI/Foundation/Base/ModBrowser/ModioCommonModBrowserBaseStyle.h"
#include "Templates/SubclassOf.h"
#include "ModioCommonModBrowserStyle.generated.h"

class UModioCommonSearchViewBase;
class UModioCommonQuickAccessViewBase;
class UModioCommonModListBase;
class UModioCommonTabButtonBase;
class UModioCommonTabButtonStyle;

/**
 * The style of the Mod Browser within the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonModBrowserStyle : public UModioCommonModBrowserBaseStyle
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "FeaturedCollection")
	TSubclassOf<UModioCommonTabButtonBase> TabButtonClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "FeaturedCollection")
	TSubclassOf<UModioCommonTabButtonStyle> TabButtonStyle;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Featured")
	TSubclassOf<UModioCommonModListBase> FeaturedViewClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Collection")
	TSubclassOf<UModioCommonModListBase> CollectionViewClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Quick Access")
	TSubclassOf<UModioCommonQuickAccessViewBase> QuickAccessViewClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Search")
	TSubclassOf<UModioCommonSearchViewBase> SearchViewClass;
};
