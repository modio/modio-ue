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
#include "ModioCommonCollectionViewStyle.generated.h"

class UModioCommonButtonStyle;
class UModioCommonTextStyle;
class UModioCommonListViewStyle;
class UModioCommonTabButtonBase;
class UModioCommonTabButtonStyle;

/**
 * The style of the Collection View within the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonCollectionViewStyle : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	TSubclassOf<UModioCommonListViewStyle> ModListStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	TSubclassOf<UModioCommonTextStyle> NumOfDownloadingModsTextStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	TSubclassOf<UModioCommonTextStyle> InstalledModsLabelTextStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	TSubclassOf<UModioCommonTextStyle> InstalledModsDescriptionTextStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	TSubclassOf<UModioCommonTextStyle> NumOfInstalledModsTextStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	TSubclassOf<UModioCommonTextStyle> ErrorsTextStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	TSubclassOf<UModioCommonTextStyle> NumOfErrorsTextStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	TSubclassOf<UModioCommonButtonStyle> FetchUpdateButtonStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	TSubclassOf<UModioCommonButtonStyle> FilterButtonStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	TSubclassOf<UModioCommonTextStyle> FilterCounterTextStyle;
};
