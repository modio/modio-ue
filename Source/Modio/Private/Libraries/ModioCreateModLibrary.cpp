/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "Libraries/ModioCreateModLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioCreateModLibrary)

void UModioCreateModLibrary::SetInitialVisibility_DEPRECATED(FModioCreateModParams& In, bool InitialVisibility)
{
	In.bVisible_DEPRECATED = InitialVisibility;
}

void UModioCreateModLibrary::SetInitialVisibility(FModioCreateModParams& In, EModioObjectVisibilityFlags InitialVisibility)
{
	In.Visibility = (uint8)InitialVisibility;
}

void UModioCreateModLibrary::SetDescription(FModioCreateModParams& In, FString Description)
{
	In.Description = Description;
}

void UModioCreateModLibrary::SetHomepageURL(FModioCreateModParams& In, FString HomepageURL)
{
	In.HomepageURL = HomepageURL;
}

void UModioCreateModLibrary::SetMaturityFlags(FModioCreateModParams& In, EModioMaturityFlags MaturityFlags)
{
	In.MaturityFlags = MaturityFlags;
}

void UModioCreateModLibrary::SetCommunityOptionsFlags(FModioCreateModParams& In, EModioModCommunityOptionsFlags CommunityOptionsFlags)
{
	In.CommunityOptionsFlags = CommunityOptionsFlags;
}

void UModioCreateModLibrary::SetMetadataBlob(FModioCreateModParams& In, FString MetadataBlob)
{
	In.MetadataBlob = MetadataBlob;
}

void UModioCreateModLibrary::SetModFileMetadataBlob(FModioCreateModFileParams& In, FString MetadataBlob)
{
	In.MetadataBlob = MetadataBlob;
}

void UModioCreateModLibrary::SetModfilePlatforms(FModioCreateModFileParams& In,
												 TArray<EModioModfilePlatform>& Platforms)
{
	In.ModfilePlatforms = Platforms;
}

void UModioCreateModLibrary::SetTags(FModioCreateModParams& In, TArray<FString>& Tags)
{
	In.Tags = Tags;
}

void UModioCreateModLibrary::SetVersionString(FModioCreateModFileParams& In, FString Version)
{
	In.VersionString = Version;
}

void UModioCreateModLibrary::SetChangelogString(FModioCreateModFileParams& In, FString Changelog)
{
	In.Changelog = Changelog;
}

void UModioCreateModLibrary::SetMarkAsActiveRelease(FModioCreateModFileParams& In, bool bMarkAsActiveRelease)
{
	In.bSetAsActiveRelease = bMarkAsActiveRelease;
}
