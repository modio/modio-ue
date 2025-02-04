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
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Misc/Optional.h"
#include "Types/ModioCommonTypes.h"
#include "Types/ModioReportParams.h"

#include "ModioReportLibrary.generated.h"

/**
 * @docpublic
 * @brief Utility library for creating content reports within the mod.io platform. 
 * This class contains methods for reporting various types of content (e.g., game, user, or mod).
 */
UCLASS()
class MODIO_API UModioReportLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * @docpublic
	 * @brief Creates a content report for a game.
	 * @param Game The ID of the game being reported
	 * @param Type The nature of the content report (e.g., abuse, inappropriate content)
	 * @param ReportDescription A description of why the content is being reported
	 * @param ReporterName Name of the submitting user. Recommended for DMCA reports, but may be empty
	 * @param ReporterContact Contact details of the submitting user. Recommended for DMCA reports, but may be empty
	 * @return A struct containing the report parameters
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "mod.io|Content Reporting")
	static FModioReportParams MakeReportForGame(FModioGameID Game, EModioReportType Type, FString ReportDescription,
												FString ReporterName, FString ReporterContact);

	/**
	 * @docpublic
	 * @brief Creates a content report for a user.
	 * @param User The ID of the User being reported
	 * @param Type The nature of the content report (e.g., abuse, inappropriate behavior)
	 * @param ReportDescription A description of why the content is being reported
	 * @param ReporterName Name of the submitting user. Recommended for DMCA reports, but may be empty
	 * @param ReporterContact Contact details of the submitting user. Recommended for DMCA reports, but may be empty
	 * @return A struct containing the report parameters
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "mod.io|Content Reporting")
	static FModioReportParams MakeReportForUser(FModioUserID User, EModioReportType Type, FString ReportDescription,
												FString ReporterName, FString ReporterContact);

	/**
	 * @docpublic
	 * @brief Creates a content report for a mod.
	 * @param Mod The ID of the content being reported (mod)
	 * @param Type The nature of the content report (e.g., abuse, inappropriate content)
	 * @param ReportDescription A description of why the content is being reported
	 * @param ReporterName Name of the submitting user. Recommended for DMCA reports, but may be empty
	 * @param ReporterContact Contact details of the submitting user. Recommended for DMCA reports, but may be empty
	 * @return A struct containing the report parameters
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "mod.io|Content Reporting")
	static FModioReportParams MakeReportForMod(FModioModID Mod, EModioReportType Type, FString ReportDescription,
											   FString ReporterName, FString ReporterContact);
};
