/*
*  Copyright (C) 2025 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once
#include "Internal/Convert/Optional.h"
#include "Internal/ModioConvert.h"
#include "Internal/Convert/Logo.h"
#include "Internal/Convert/User.h"
#include "ModioSDK.h"
#include "Types/ModioModCollection.h"

FORCEINLINE FModioModCollectionInfo ToUnreal(const Modio::ModCollectionInfo& In)
{
	FModioModCollectionInfo Out = {};

	Out.Id = ToUnreal(In.Id);
	Out.GameId = ToUnreal(In.GameId);
	Out.GameName = ToUnreal(In.GameName);
	Out.CollectionStatus = static_cast<EModioModServerSideStatus>(In.CollectionStatus);
	Out.Visibility = static_cast<EModioObjectVisibilityFlags>(In.Visibility);
	Out.ProfileSubmittedBy = ToUnreal(In.ProfileSubmittedBy);
	Out.Category = ToUnreal(In.Category);
	Out.ProfileDateAdded = ToUnrealDateTime(In.ProfileDateAdded);
	Out.ProfileDateUpdated = ToUnrealDateTime(In.ProfileDateUpdated);
	Out.ProfileDateLive = ToUnrealDateTime(In.ProfileDateLive);
	Out.Incomplete = ToUnreal(In.Incomplete);
	Out.ProfileMaturityOption = ToUnreal<EModioMaturityFlags, Modio::MaturityOption>(In.ProfileMaturityOption);
	Out.FileSize = FModioUnsigned64(In.FileSize);
	Out.FileSizeUncompressed = FModioUnsigned64(In.FileSizeUncompressed);

	Out.Platforms = ToUnreal<EModioModfilePlatform, Modio::ModfilePlatform>(In.Platforms);

	Out.Tags = ToUnreal<FString>(In.Tags);
	Out.Stats = ToUnreal(In.Stats);
	Out.Logo = ToUnreal(In.Logo);
	Out.ProfileName = ToUnreal(In.ProfileName);
	Out.ProfileNameId = ToUnreal(In.ProfileNameId);
	Out.ProfileSummary = ToUnreal(In.ProfileSummary);
	Out.ProfileDescription = ToUnreal(In.ProfileDescription);
	Out.ProfileDescriptionPlaintext = ToUnreal(In.ProfileDescriptionPlaintext);

	return Out;
}

FORCEINLINE FModioModCollectionInfoList ToUnreal(const Modio::ModCollectionInfoList& In)
{
	return FModioModCollectionInfoList(In);
}