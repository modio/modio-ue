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
#include "Engine/DeveloperSettings.h"
#include "Templates/SubclassOf.h"
#include "Templates/SharedPointer.h"
#include "ModioCommonFeaturedViewStyle.generated.h"

class UModioCommonGenericModEntryStyle;
class UModioCommonListViewStyle;
class UModioCommonTabButtonStyle;
class UModioCommonButtonStyle;
class UModioCommonTextStyle;
class UModioCommonTabButtonBase;
class UModioCommonDynamicImageStyle;

/**
 * @brief The style of the Featured Additional View within the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI5_API UModioCommonFeaturedAdditionalViewStyle : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	TSubclassOf<UModioCommonListViewStyle> ModListStyle;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Tab")
	TSubclassOf<UModioCommonTabButtonBase> CategoryTabButtonClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Tab")
	TSubclassOf<UModioCommonTabButtonStyle> CategoryTabButtonStyle;
};

/**
 * @brief The style of the Featured Primary View within the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI5_API UModioCommonFeaturedPrimaryViewStyle : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	TSubclassOf<UModioCommonGenericModEntryStyle> LargeModEntryStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	TSubclassOf<UModioCommonListViewStyle> ModListStyle;
};

/**
 * @brief The style of the Featured View within the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI5_API UModioCommonFeaturedViewStyle : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	TSubclassOf<UModioCommonFeaturedPrimaryViewStyle> FeaturedPrimaryViewStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	TSubclassOf<UModioCommonFeaturedAdditionalViewStyle> FeaturedAdditionalViewStyle;
};