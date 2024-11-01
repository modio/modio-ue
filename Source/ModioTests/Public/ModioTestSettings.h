/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "Types/ModioCommonTypes.h"
#include "Types/ModioInitializeOptions.h"

#include "ModioTestSettings.generated.h"

/**
 *
 */
UCLASS(Config = ModioTest, defaultconfig)
class MODIOTESTS_API UModioTestSettings : public UObject
{
	GENERATED_BODY()
	public:
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, config, Category = "Automation Testing")
	FModioInitializeOptions AutomationTestOptions;

	UPROPERTY(EditDefaultsOnly, config, Category = "Automation Testing")
	FString AutomationSessionID;
#endif
};
