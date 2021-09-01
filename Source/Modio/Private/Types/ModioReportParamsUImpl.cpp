/* 
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *  
 *  This file is part of the mod.io UE4 Plugin.
 *  
 *  Distributed under the MIT License. (See accompanying file LICENSE or 
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *   
 */

#include "ModioSDK.h"
#include "Internal/ModioConvert.h"
#include "Types/ModioCommonTypes.h"
#include "Types/ModioReportParams.h"

FModioReportParams::FModioReportParams(FModioGameID Game, EModioReportType Type, FString ReportDescription,
			 TOptional<FString> ReporterName, TOptional<FString> ReporterContact)
	: FModioReportParams(ToModio(Game), FModioReportParams::ResourceType::Game, Type, ReportDescription, ReporterName,
				   ReporterContact)
{}

FModioReportParams::FModioReportParams(FModioUserID User, EModioReportType Type, FString ReportDescription,
			 TOptional<FString> ReporterName, TOptional<FString> ReporterContact)
	: FModioReportParams(ToModio(User), FModioReportParams::ResourceType::User, Type, ReportDescription, ReporterName,
				   ReporterContact)
{}

FModioReportParams::FModioReportParams(FModioModID Mod, EModioReportType Type, FString ReportDescription,
				   TOptional<FString> ReporterName,
			 TOptional<FString> ReporterContact)
	: FModioReportParams(ToModio(Mod), FModioReportParams::ResourceType::Mod, Type, ReportDescription, ReporterName,
				   ReporterContact)
{}

FModioReportParams::FModioReportParams(int64 ResourceID, ResourceType ReportedResourceType, EModioReportType Type,
				   FString ReportDescription,
			 TOptional<FString> ReporterName, TOptional<FString> ReporterContact)
	: ReportedResourceType(ReportedResourceType),
	  ResourceID(ResourceID),
	  Type(Type),
	  ReporterName(ReporterName),
	  ReporterContact(ReporterContact),
	  ReportDescription(ReportDescription)
{}

FModioReportParams::FModioReportParams()
	: ReportedResourceType(FModioReportParams::ResourceType::Game),
	  ResourceID(ToModio(FModioGameID::InvalidGameID())),
	  Type(EModioReportType::Other),
	  ReporterName(),
	  ReporterContact(),
	  ReportDescription("")
{}