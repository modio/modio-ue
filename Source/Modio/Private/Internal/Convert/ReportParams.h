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
#include "Internal/ModioConvert.h"
#include "ModioSDK.h"
#include "Types/ModioReportParams.h"

FORCEINLINE Modio::ReportType ToModio(const EModioReportType& In)
{
	return static_cast<Modio::ReportType>(In);
}

FORCEINLINE Modio::ReportParams ToModio(const FModioReportParams& In)
{
	Modio::Optional<std::string> ReporterName;
	Modio::Optional<std::string> ReporterContact;

	if (In.ReporterName && !In.ReporterName->TrimStartAndEnd().IsEmpty())
	{
		ReporterName = ToModio(In.ReporterName.GetValue());
	}

	if (In.ReporterContact && !In.ReporterContact->TrimStartAndEnd().IsEmpty())
	{
		ReporterContact = ToModio(In.ReporterContact.GetValue());
	}

	switch (In.ReportedResourceType)
	{
		case FModioReportParams::ResourceType::Game:
			return Modio::ReportParams(Modio::GameID(In.ResourceID), ToModio(In.Type), ToModio(In.ReportDescription),
									   ReporterName, ReporterContact);
			break;
		case FModioReportParams::ResourceType::Mod:
			return Modio::ReportParams(Modio::ModID(In.ResourceID), ToModio(In.Type), ToModio(In.ReportDescription),
									   ReporterName, ReporterContact);
			break;
		case FModioReportParams::ResourceType::User:
			return Modio::ReportParams(Modio::UserID(In.ResourceID), ToModio(In.Type), ToModio(In.ReportDescription),
									   ReporterName, ReporterContact);
			break;
	}
	// There's no sensible default for reported resource type, so returning params with empty description should trip
	// the data validation checks on the server and give them the response we want
	return Modio::ReportParams(Modio::GameID(-1), ToModio(In.Type), "", {}, {});
}
