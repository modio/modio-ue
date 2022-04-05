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

#include "Types/ModioCommonTypes.h"
#include "Types/ModioInitializeOptions.h"

#include "ModioSettings.generated.h"

/** @brief Settings that can be configured per project that generally don't change during the run of a game */
UCLASS(Config = Game, defaultconfig)
class MODIO_API UModioSettings : public UObject
{
	GENERATED_BODY()
public:
	UModioSettings();

	/** Game id that can be found in your game configuration page on the mod.io site. Unique per environment  */
	UPROPERTY(EditDefaultsOnly, config, Category = "Project")
	int64 GameId;

	/** ApiKey that can be found in your game configuration page on the mod.io site. Unique per environment  */
	UPROPERTY(EditDefaultsOnly, config, Category = "Project")
	FString ApiKey;

	/** If your game is configured to be running on either mod.io or test.mod.io. Recommended is to first create your
	 * game on test.mod.io as only you and your team can see it there */
	UPROPERTY(EditDefaultsOnly, config, Category = "Project")
	EModioEnvironment Environment = EModioEnvironment::Test;

	/** What's the default log level you are interested in */
	UPROPERTY(EditDefaultsOnly, config, Category = "Project")
	EModioLogLevel LogLevel;

	/** The default portal to use.*/
	UPROPERTY(EditDefaultsOnly, config, Category = "Project")
	EModioPortal DefaultPortal = EModioPortal::None;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, config, Category = "Project|Automation Testing", meta = (ShowOnlyInnerProperties))
	FModioInitializeOptions AutomationTestOptions;

	UPROPERTY(EditDefaultsOnly, config, Category = "Project|Automation Testing")
	FString AutomationSessionID;
#endif

private:
#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
