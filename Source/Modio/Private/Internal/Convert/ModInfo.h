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
#include "Internal/Convert/FileMetadata.h"
#include "Internal/Convert/Metadata.h"
#include "Internal/Convert/ModStats.h"
#include "Internal/Convert/ModTag.h"
#include "Internal/Convert/URLList.h"
#include "Internal/Convert/User.h"
#include "Internal/ModioConvert.h"
#include "ModioSDK.h"
#include "Types/ModioModInfo.h"
#include "Types/ModioUnsigned64.h"

FORCEINLINE FModioModInfo ToUnreal(const Modio::ModInfo& In)
{
	FModioModInfo Out;
	Out.ModId = ToUnreal(In.ModId);
	Out.ProfileName = ToUnreal(In.ProfileName);
	Out.ProfileSummary = ToUnreal(In.ProfileSummary);
	Out.ProfileDescription = ToUnreal(In.ProfileDescription);
	Out.ProfileDescriptionPlaintext = ToUnreal(In.ProfileDescriptionPlaintext);
	Out.ProfileURL = ToUnreal(In.ProfileURL);
	Out.ProfileSubmittedBy = ToUnreal(In.ProfileSubmittedBy);
	Out.ProfileDateAdded = ToUnrealDateTime(In.ProfileDateAdded);
	Out.ProfileDateUpdated = ToUnrealDateTime(In.ProfileDateUpdated);
	Out.ProfileDateLive = ToUnrealDateTime(In.ProfileDateLive);
	Out.ProfileMaturityOption = ToUnreal<EModioMaturityFlags, Modio::MaturityOption>(In.ProfileMaturityOption);
	Out.Visibility = (EModioObjectVisibilityFlags) In.Visibility;
	Out.Dependencies = In.Dependencies;

	Out.MetadataBlob = FString(In.MetadataBlob.c_str()); // Converting verbatim rather than via TCHAR as ToUnreal does

	if (In.FileInfo.has_value())
	{
		Out.FileInfo = ToUnreal(In.FileInfo.value());
	}

	Out.MetadataKvp = ToUnreal<FModioMetadata>(In.MetadataKvp);
	Out.Tags = ToUnreal<FModioModTag>(In.Tags);
	Out.NumGalleryImages = ToUnreal(In.NumGalleryImages);
	Out.YoutubeURLs = ToUnreal(In.YoutubeURLs);
	Out.SketchfabURLs = ToUnreal(In.SketchfabURLs);
	Out.Stats = ToUnreal(In.Stats);
	Out.ModStatus = (EModioModServerSideStatus) In.ModStatus;
	Out.Price = FModioUnsigned64(In.Price);
	return Out;
}
