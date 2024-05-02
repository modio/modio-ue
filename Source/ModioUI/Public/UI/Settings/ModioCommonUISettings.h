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
#include "Params/ModioCommonAuthParams.h"
#include "Params/ModioCommonErrorWithRetryParams.h"
#include "Params/ModioCommonFilteredModListParams.h"
#include "Params/ModioCommonModBrowserParams.h"
#include "Params/ModioCommonModDetailsParams.h"
#include "Params/ModioCommonModEntryParams.h"
#include "Params/ModioCommonModGalleryParams.h"
#include "Params/ModioCommonModOperationTrackerParams.h"
#include "Params/ModioCommonQuickAccessParams.h"
#include "Params/ModioCommonReportParams.h"
#include "Params/ModioCommonSearchParams.h"
#include "Params/ModioCommonUserProfileWidgetParams.h"
#include "UI/Settings/ModioCommonDefines.h"

#include "ModioCommonUISettings.generated.h"

/**
 * Settings for Mod.io Common UI
 */
UCLASS(Config = "ModioCommonUISettings", DefaultConfig, meta = (DisplayName = "Modio Common UI Settings"))
class MODIOUI_API UModioCommonUISettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	/** Enables a user interface for enabling and disabling mods  */
	UPROPERTY(Config, EditDefaultsOnly, Category = "Settings")
	bool bEnableCollectionModDisableUI = false;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Settings|Auth")
	FModioCommonAuthParamsSettings AuthParams;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Settings|Auth")
	FModioCommonEmailAuthCodeParamsSettings EmailAuthCodeParams;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Settings|Auth")
	FModioCommonEmailAuthLoadingParamsSettings EmailAuthLoadingParams;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Settings|Auth")
	FModioCommonEmailAuthParamsSettings EmailAuthParams;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Settings|Auth")
	FModioCommonTermsOfUseParamsSettings TermsOfUseParams;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Settings|FilteredModList")
	FModioCommonFilteredModListParams FilteredModListParams;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Settings|ModBrowser")
	FModioCommonCollectionParamsSettings CollectionParams;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Settings|ModBrowser")
	FModioCommonFeaturedParamsSettings FeaturedParams;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Settings|ModBrowser")
	FModioCommonModBrowserParamsSettings ModBrowserParams;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Settings|ModDetails")
	FModioCommonModDetailsParamsSettings ModDetailsParams;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Settings|ModEntry")
	FModioCommonModEntryParamsSettings ModEntryParams;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Settings|ModGallery")
	FModioCommonModGalleryParamsSettings ModGalleryParams;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Settings|ModOperationTracker")
	FModioCommonModOperationTrackerParamsSettings ModOperationTrackerParams;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Settings|QuickAccess")
	FModioCommonQuickAccessParamsSettings QuickAccessParams;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Settings|Report")
	FModioCommonReportParamsSettings ReportParams;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Settings|Report")
	FModioCommonReportReasonParamsSettings ReportReasonParams;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Settings|Report")
	FModioCommonReportEmailParamsSettings ReportEmailParams;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Settings|Report")
	FModioCommonReportMessageParamsSettings ReportMessageParams;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Settings|Report")
	FModioCommonReportSummaryParamsSettings ReportSummaryParams;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Settings|Search")
	FModioCommonSearchParamsSettings SearchParams;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Settings|UserProfile")
	FModioCommonUserProfileWidgetParamsSettings UserProfileParams;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Settings|UserProfile")
	FModioCommonErrorWithRetryParamsSettings ErrorWithRetryParams;

	// Hides the bottom action bar in the ModioCommonModBrowser automatically when MouseAndKeyboard is the current input type
	UPROPERTY(Config, EditDefaultsOnly, Category = "Settings|ModBrowser")
	bool bHideActionBarDuringMouseAndKeyboardInput = false;

	// Whether or not to show the login/logout button in the QuickAccessTabView
	UPROPERTY(Config, EditDefaultsOnly, Category = "Settings|Auth")
	bool bShowAuthButton = true;
	
	// Begin UDeveloperSettings Interface
	virtual FName GetCategoryName() const override { return ModioCommonCategoryName; }
	// End UDeveloperSettings Interface
};
