/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "Library/ModioSubmissionExtensionLibrary.h"

#include "HAL/FileManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "ModioEx.h"
#include "ModioSubsystem.h"

static const FString ModFileName(TEXT("Mod.uasset"));

bool UModioSubmissionExtensionLibrary::LoadModFileToMemory(UModioSubsystem* Target, FModioModID ModId,
														   TArray<uint8>& ModData)
{
	auto InstalledMods = Target->QueryUserInstallations(false);

	if (!InstalledMods.Contains(ModId))
	{
		UE_LOG(LogModioEx, Error, TEXT("Mod with ID %s was not installed"), *ModId.ToString());
		return false;
	}

	const FString ModPath = InstalledMods[ModId].GetPath();
	const FString ModFilePath = FPaths::Combine(*ModPath, *ModFileName);

	if (FFileHelper::LoadFileToArray(ModData, *ModFilePath))
	{
		return true;
	}

	UE_LOG(LogModioEx, Error, TEXT("Could not load ModId %s from path %s"), *ModId.ToString(), *ModFilePath);
	return false;
}

bool UModioSubmissionExtensionLibrary::K2_LoadModFileToMemory(UModioSubsystem* Target, FModioModID ModId,
															  TArray<uint8>& ModData)
{
	return LoadModFileToMemory(Target, ModId, ModData);
}

void UModioSubmissionExtensionLibrary::SubmitNewModFileForModFromMemory(UModioSubsystem* Target, FModioModID Mod,
																		FModioCreateModFileMemoryParams Params)
{
	const FString ModTempDir =
		FString(FPlatformProcess::UserTempDir()) + TEXT("ModioTemp/") + FGuid::NewGuid().ToString();
	const FString FilePath = FPaths::Combine(*ModTempDir, *ModFileName);

	// Ensure that our temp folder exists
	IFileManager::Get().MakeDirectory(*ModTempDir);

	if (FFileHelper::SaveArrayToFile(Params.ModMemory, *FilePath))
	{
		// Transform params
		FModioCreateModFileParams CreateModFileParams;
		CreateModFileParams.Changelog = Params.Changelog;
		CreateModFileParams.MetadataBlob = Params.MetadataBlob;
		CreateModFileParams.PathToModRootDirectory = FPaths::ConvertRelativePathToFull(*ModTempDir) + TEXT("/");
		CreateModFileParams.VersionString = Params.VersionString;
		CreateModFileParams.bSetAsActiveRelease = Params.bSetAsActiveRelease;

		// Submit the file to mod.io for this mod
		Target->SubmitNewModFileForMod(Mod, CreateModFileParams);
	}
}

void UModioSubmissionExtensionLibrary::K2_SubmitNewModFileForModFromMemory(UModioSubsystem* Target, FModioModID Mod,
																		   FModioCreateModFileMemoryParams Params)
{
	SubmitNewModFileForModFromMemory(Target, Mod, Params);
}
