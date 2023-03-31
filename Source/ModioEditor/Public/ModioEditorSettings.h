/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#pragma once

#include "ModioEditorSettings.generated.h"

UCLASS(Config = Editor, defaultconfig)
class UModioEditorSettings : public UObject
{
	GENERATED_BODY()
public:
	UModioEditorSettings(const FObjectInitializer& Initializer);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Config, Category = "modio Editor")
	bool bShowGettingStartedOnStartup = true;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Config, Category = "modio Editor")
	TSoftObjectPtr<class UEditorUtilityWidgetBlueprint> GettingStartedWidget;
};

