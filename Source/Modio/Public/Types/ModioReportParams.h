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
#include "Misc/EnumRange.h"

#include "ModioReportParams.generated.h"

namespace Modio
{
	class ReportParams;
}

/**
* Enumeration for the type of report that a mod can receive
**/
UENUM(BlueprintType)
enum class EModioReportType : uint8
{
	/** A generic mod report **/
	Generic = 0,
	
	/** Digital Millennium Copyright Act mod report **/
	DMCA = 1,

	/** Not working mod report **/
	NotWorking = 2,

	/** Rude content mod report **/
	RudeContent = 3,

	/** Illegal content mod report **/
	IllegalContent = 4,

	/** Stolen content mod report **/
	StolenContent = 5,

	/** False information mod report **/
	FalseInformation = 6,

	/** Other type of mod report **/
	Other = 7
};
ENUM_RANGE_BY_FIRST_AND_LAST(EModioReportType, EModioReportType::Generic, EModioReportType::Other);

/**
* Struct type to store the parameters used to report a mod
**/
USTRUCT(BlueprintType)
struct MODIO_API FModioReportParams
{
	GENERATED_BODY();
	
    /**
    * Default constructor without parameters
    **/
	FModioReportParams();

	/// @docpublic
	/// @brief Creates a content report for a game.
	/// @param Mod The ID of the content being reported
	/// @param Type The nature of the content report
	FModioReportParams(struct FModioModID Mod, EModioReportType Type);

	/// @docpublic
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
	
	/**
	* Enumeration for a type of resource
	**/
	enum class ResourceType : uint8
	{
		/** Resource is a game **/
		Game,

		/** Resource is a mod **/
		Mod,

		/** Resource is a user **/
		User
	};

	/**
	* Stored property to the resource type this report contains
	**/
	ResourceType ReportedResourceType;
	
	/**
	* Stored property for the resource ID
	**/
	int64 ResourceID;

	/**
	* Stored property for the report type 
	**/
	EModioReportType Type;
	
	/**
	* Optional stored property of the reporter name
	**/
	TOptional<FString> ReporterName;

	/**
	* Optional stored property of the reporter contact email
	**/
	TOptional<FString> ReporterContact;

	/**
	* Stored property for the report description
	**/
	FString ReportDescription;

	/**
    * Convenience constructor with the basic parameters
	* @param ResourceID The ID for the resource to report
	* @param ReportedResourceType The type of the resource
	* @param Type What kind of report is
	* @param ReportDescription Details of the report
	* @param ReporterName Who is reporting this resource
	* @param ReportContact How to contact the reporter
    **/
	FModioReportParams(int64 ResourceID, ResourceType ReportedResourceType, EModioReportType Type,
				 FString ReportDescription, TOptional<FString> ReporterName,
				 TOptional<FString> ReporterContact);

	/**
	* Bridge method to transform between FModio and Modio:: Report Params
	**/
	friend Modio::ReportParams ToModio(const FModioReportParams& In);
};