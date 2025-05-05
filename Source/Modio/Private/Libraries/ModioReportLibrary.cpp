/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "Libraries/ModioReportLibrary.h"
#include "Types/ModioCommonTypes.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioReportLibrary)

FModioReportParams UModioReportLibrary::MakeReportForGame(FModioGameID Game, EModioReportType Type,
														  FString ReportDescription, FString ReporterName,
														  FString ReporterContact)
{
	return FModioReportParams(Game, Type, ReportDescription, ReporterName, ReporterContact);
}

FModioReportParams UModioReportLibrary::MakeReportForUser(FModioUserID User, EModioReportType Type,
														  FString ReportDescription, FString ReporterName,
														  FString ReporterContact)
{
	return FModioReportParams(User, Type, ReportDescription, ReporterName, ReporterContact);
}

FModioReportParams UModioReportLibrary::MakeReportForMod(FModioModID Mod, EModioReportType Type,
														 FString ReportDescription, FString ReporterName,
														 FString ReporterContact)
{
	return FModioReportParams(Mod, Type, ReportDescription, ReporterName, ReporterContact);
}
