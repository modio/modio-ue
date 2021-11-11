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

#include "ModioReportParams.generated.h"

namespace Modio
{
	class ReportParams;
}

UENUM(BlueprintType)
enum class EModioReportType : uint8
{
	Generic = 0,
	DMCA = 1,
	NotWorking = 2,
	RudeContent = 3,
	IllegalContent = 4,
	StolenContent = 5,
	FalseInformation = 6,
	Other = 7
};


USTRUCT(BlueprintType)
struct MODIO_API FModioReportParams
{
	GENERATED_BODY();


	FModioReportParams();

	/// @brief Creates a content report for a game.
	/// @param Game The ID of the game being reported
	/// @param Type The nature of the content report
	/// @param ReportDescription A description of why the content is being reported
	/// @param ReporterName Name of the submitting user. Recommended for DMCA reports, but may be empty
	/// @param ReporterContact Contact details of the submitting user. Recommended for DMCA reports, but may be
	/// empty
	FModioReportParams(struct FModioGameID Game, EModioReportType Type, FString ReportDescription,
							TOptional<FString> ReporterName, TOptional<FString> ReporterContact);

	/// @docpublic
	/// @brief Creates a content report for a game.
	/// @param User The ID of the User being reported
	/// @param Type The nature of the content report
	/// @param ReportDescription A description of why the content is being reported
	/// @param ReporterName Name of the submitting user. Recommended for DMCA reports, but may be empty
	/// @param ReporterContact Contact details of the submitting user. Recommended for DMCA reports, but may be
	/// empty
	FModioReportParams(struct FModioUserID User, EModioReportType Type, FString ReportDescription,
							TOptional<FString> ReporterName, TOptional<FString> ReporterContact);

	/// @docpublic
	/// @brief Creates a content report for a game.
	/// @param Mod The ID of the content being reported
	/// @param Type The nature of the content report
	/// @param ReportDescription A description of why the content is being reported
	/// @param ReporterName Name of the submitting user. Recommended for DMCA reports, but may be empty
	/// @param ReporterContact Contact details of the submitting user. Recommended for DMCA reports, but may be
	/// empty
	FModioReportParams(struct FModioModID Mod, EModioReportType Type, FString ReportDescription,
							TOptional<FString> ReporterName, TOptional<FString> ReporterContact);

private:
	

	enum class ResourceType : uint8
	{
		Game,
		Mod,
		User
	};
	ResourceType ReportedResourceType;
	
	int64 ResourceID;

	EModioReportType Type;
	
	TOptional<FString> ReporterName;

	TOptional<FString> ReporterContact;

	FString ReportDescription;

	FModioReportParams(int64 ResourceID, ResourceType ReportedResourceType, EModioReportType Type,
				 FString ReportDescription, TOptional<FString> ReporterName,
				 TOptional<FString> ReporterContact);

	friend Modio::ReportParams ToModio(const FModioReportParams& In);
};