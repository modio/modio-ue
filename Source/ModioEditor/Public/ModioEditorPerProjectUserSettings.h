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

#include "CoreMinimal.h"

#include "ModioEditorPerProjectUserSettings.generated.h"

UCLASS(Config = EditorPerProjectUserSettings)
class UModioEditorPerProjectUserSettings : public UObject
{
	GENERATED_BODY()

public:

	UModioEditorPerProjectUserSettings(const FObjectInitializer& Initializer);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Config, Category = "modio Editor")
	bool bShowGettingStartedOnStartup = true;
};
