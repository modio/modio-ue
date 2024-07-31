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
#include "Types/ModioFilterParams.h"
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

	/** Run the mod.io plugin on a managed background thread. Enabling this will spin up a new thread which
	 * automatically calls RunPendingHandlers for you. Refer to the documentation for more information about this
	 * option. */
	UPROPERTY(EditDefaultsOnly, config, Category = "Project")
	bool bUseBackgroundThread = false;

	// TODO: These properties probably should go into ModioUISettings instead

	/**
	 * The maximum number of reattempts the Mod Menu will make on any network operation
	 */
	UPROPERTY(EditDefaultsOnly, config, Category = "Network")
	int32 GlobalRetryLimit = 3;

	/**
	 * The string table asset which stores localization data for the plugin (user-facing strings for enums, etc)
	 */
	UPROPERTY(EditDefaultsOnly, config, Category = "Localization", meta = (MetaClass = StringTable))
	FSoftObjectPath LocalizationStringTable;

	/**
	 * Named sets of filter parameters for use in the default PresetFilterSelector UI component
	 */
	UPROPERTY(EditDefaultsOnly, config, Category = "UI Configuration")
	TArray<FModioPresetFilterParams> PresetNamedFilters;

	/**
	 * If enabled, default implementations of Monetization-related UI components will be visible and interactable in mod.io UI templates
	 */
	UPROPERTY(EditDefaultsOnly, config, Category = "Feature Flags", meta=(DisplayName="Enable Monetization widgets in UI"))
	bool bEnableMonetizationFeature;

	/**
	 * If enabled, the UUGCSubsystem can manage mod enabled/disable state, if a valid IModEnabledStateProvider has been set via SetUGCEnabledStateProvider so you can persist the state appropriately
	 * If this is disabled, enable/disable widgets in the UI will be hidden
	 */
	UPROPERTY(EditDefaultsOnly, config, Category = "Feature Flags", meta = (DisplayName = "Enable Mod Enable/Disable support"))
	bool bEnableModEnableDisableFeature;

	/**
	 * If enabled, widgets in mod.io UI templates will allow users to attempt to downvote mods. If downvoting is not enabled on the mod.io dashboards such votes will not be recorded
	 */
	UPROPERTY(EditDefaultsOnly, config, Category = "Feature Flags", meta=(DisplayName="Enable Mod Downvoting in UI"))
	bool bEnableModDownvoteFeature;


private:
#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
