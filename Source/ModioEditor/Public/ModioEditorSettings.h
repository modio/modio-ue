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

#include "Types/ModioGettingStartedEntry.h"

#include "ModioEditorSettings.generated.h"

UCLASS(Config = Editor, defaultconfig)
class UModioEditorSettings : public UObject
{
	GENERATED_BODY()

public:

	UModioEditorSettings(const FObjectInitializer& Initializer);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Config, Category = "modio Editor")
	bool bDisplayToolsMenuItem = true;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Config, Category = "modio Editor|Getting Started")
	TSoftObjectPtr<class UEditorUtilityWidgetBlueprint> GettingStartedWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = "modio Editor|Getting Started")
	TSet<FModioGettingStartedEntry> GettingStartedEntries;

	/* This set is populated automatically by submodules on load */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "modio Editor|Getting Started")
	TSet<FModioGettingStartedEntry> SubmoduleGettingStartedEntries;
};
