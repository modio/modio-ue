/* 
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *  
 *  This file is part of the mod.io UE4 Plugin.
 *  
 *  Distributed under the MIT License. (See accompanying file LICENSE or 
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *   
 */

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Misc/Optional.h"
#include "Types/ModioReportParams.h"
#include "Types/ModioCommonTypes.h"

#include "ModioReportLibrary.generated.h"


/**
 *
 */
UCLASS()
class MODIO_API UModioReportLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	/// @brief Creates a content report for a game.
	/// @param Game The ID of the game being reported
	/// @param Type The nature of the content report
	/// @param ReportDescription A description of why the content is being reported
	/// @param ReporterName Name of the submitting user. Recommended for DMCA reports, but may be empty
	/// @param ReporterContact Contact details of the submitting user. Recommended for DMCA reports, but may be
	/// empty
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "mod.io|Content Reporting")
	static FModioReportParams MakeReportForGame(FModioGameID Game, EModioReportType Type,
												FString ReportDescription, FString ReporterName,
												FString ReporterContact);

	/// @docpublic
	/// @brief Creates a content report for a game.
	/// @param User The ID of the User being reported
	/// @param Type The nature of the content report
	/// @param ReportDescription A description of why the content is being reported
	/// @param ReporterName Name of the submitting user. Recommended for DMCA reports, but may be empty
	/// @param ReporterContact Contact details of the submitting user. Recommended for DMCA reports, but may be
	/// empty
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "mod.io|Content Reporting")
	static FModioReportParams MakeReportForUser(FModioUserID User, EModioReportType Type,
												FString ReportDescription, FString ReporterName,
												FString ReporterContact);

	/// @docpublic
	/// @brief Creates a content report for a game.
	/// @param Mod The ID of the content being reported
	/// @param Type The nature of the content report
	/// @param ReportDescription A description of why the content is being reported
	/// @param ReporterName Name of the submitting user. Recommended for DMCA reports, but may be empty
	/// @param ReporterContact Contact details of the submitting user. Recommended for DMCA reports, but may be
	/// empty
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "mod.io|Content Reporting")
	static FModioReportParams MakeReportForMod(FModioModID Mod, EModioReportType Type, FString ReportDescription,
											   FString ReporterName, FString ReporterContact);
};
