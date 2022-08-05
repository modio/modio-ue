// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Types/ModioCommonTypes.h"
#include "Types/ModioCreateModFileParams.h"
#include "Types/ModioCreateModParams.h"

#include "ModioCreateModLibrary.generated.h"

/**
 *
 */
UCLASS()
class MODIO_API UModioCreateModLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "mod.io|Create Mod Params")
	void SetInitialVisibility(UPARAM(ref) FModioCreateModParams& In, bool InitialVisibility);

	UFUNCTION(BlueprintCallable, Category = "mod.io|Create Mod Params")
	void SetDescription(UPARAM(ref) FModioCreateModParams& In, FString Description);

	UFUNCTION(BlueprintCallable, Category = "mod.io|Create Mod Params")
	void SetHomepageURL(UPARAM(ref) FModioCreateModParams& In, FString HomepageURL);

	UFUNCTION(BlueprintCallable, Category = "mod.io|Create Mod Params")
	void SetMetadataBlob(UPARAM(ref) FModioCreateModParams& In, FString MetadataBlob);

	UFUNCTION(BlueprintCallable, Category = "mod.io|Create Mod Params")
	void SetTags(UPARAM(ref) FModioCreateModParams& In, UPARAM(ref) TArray<FString>& Tags);

	UFUNCTION(BlueprintCallable, Category = "mod.io|Create Mod File Params")
	void SetVersionString(UPARAM(ref) FModioCreateModFileParams& In, FString Version);

	UFUNCTION(BlueprintCallable, Category = "mod.io|Create Mod File Params")
	void SetChangelogString(UPARAM(ref) FModioCreateModFileParams& In, FString Changelog);

	UFUNCTION(BlueprintCallable, Category = "mod.io|Create Mod File Params")
	void SetMarkAsActiveRelease(UPARAM(ref) FModioCreateModFileParams& In, bool bMarkAsActiveRelease);

	UFUNCTION(BlueprintCallable, Category = "mod.io|Create Mod File Params")
	void SetModFileMetadataBlob(UPARAM(ref) FModioCreateModFileParams& In, FString MetadataBlob);

	UFUNCTION(BlueprintCallable, Category = "mod.io|Create Mod File Params")
	void SetModfilePlatforms(UPARAM(ref) FModioCreateModFileParams& In,
							 UPARAM(ref) TArray<EModioModfilePlatform>& Platforms);
};
