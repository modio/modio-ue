/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "Library/ModioSubmissionExtensionLibrary.h"

#include "ImageUtils.h"
#include "HAL/FileManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "ModioEx.h"
#include "ModioSubsystem.h"
#include "Engine/Texture2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Serialization/BufferArchive.h"
#include "Engine/Engine.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioSubmissionExtensionLibrary)

static const FString ModFileName(TEXT("Mod.modfile"));
static const FString LogoFileName(TEXT("ModLogo.png"));

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

void UModioSubmissionExtensionLibrary::SubmitNewModFromMemoryAsync(FModioModCreationHandle Handle, FModioCreateModParams Params, TArray<uint8> PngData, FOnSubmitNewModDelegateFast Callback)
{
	const FString LogoTempDir =
		FString(FPlatformProcess::UserTempDir()) + TEXT("ModioTemp/") + FGuid::NewGuid().ToString();
	const FString FilePath = FPaths::Combine(*LogoTempDir, *LogoFileName);

	if (FFileHelper::SaveArrayToFile(PngData, *FilePath))
	{
		Params.PathToLogoFile = FilePath;

		GEngine->GetEngineSubsystem<UModioSubsystem>()->SubmitNewModAsync(Handle, Params,Callback);
	}
}

void UModioSubmissionExtensionLibrary::K2_SubmitNewModFromMemoryAsync(FModioModCreationHandle Handle, FModioCreateModParams Params, TArray<uint8> PngData, FOnSubmitNewModDelegate Callback)
{
	SubmitNewModFromMemoryAsync(
		Handle, Params, PngData,
		FOnSubmitNewModDelegateFast::CreateLambda([Callback](FModioErrorCode ec, TOptional<FModioModID> NewModID) {
			Callback.ExecuteIfBound(ec, FModioOptionalModID {NewModID});
		}));
}

void UModioSubmissionExtensionLibrary::K2_SubmitModChangesFromMemoryAsync(FModioModID Mod, FModioEditModParams Params, TArray<uint8> PngData, FOnGetModInfoDelegate Callback)
{
	SubmitModChangesFromMemoryAsync(
		Mod, Params, PngData,
		FOnGetModInfoDelegateFast::CreateLambda([Callback](FModioErrorCode ec, TOptional<FModioModInfo> MaybeModInfo) {
			Callback.ExecuteIfBound(ec, FModioOptionalModInfo {MaybeModInfo});
		}));
}

void UModioSubmissionExtensionLibrary::SubmitModChangesFromMemoryAsync(FModioModID Mod, FModioEditModParams Params, TArray<uint8> PngData, FOnGetModInfoDelegateFast Callback)
{
	const FString LogoTempDir =
		FString(FPlatformProcess::UserTempDir()) + TEXT("ModioTemp/") + FGuid::NewGuid().ToString();
	const FString FilePath = FPaths::Combine(*LogoTempDir, *LogoFileName);

	if (FFileHelper::SaveArrayToFile(PngData, *FilePath))
	{
		Params.LogoPath = FilePath;

		GEngine->GetEngineSubsystem<UModioSubsystem>()->SubmitModChangesAsync(Mod, Params, Callback);
	}
}
