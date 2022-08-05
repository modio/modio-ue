// Fill out your copyright notice in the Description page of Project Settings.

#include "Libraries/ModioCreateModLibrary.h"

void UModioCreateModLibrary::SetInitialVisibility(FModioCreateModParams& In, bool InitialVisibility) 
{
	In.bVisible = InitialVisibility;
}

void UModioCreateModLibrary::SetDescription(FModioCreateModParams& In, FString Description) 
{
	In.Description = Description;
}

void UModioCreateModLibrary::SetHomepageURL(FModioCreateModParams& In, FString HomepageURL) 
{
	In.HomepageURL = HomepageURL;
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
