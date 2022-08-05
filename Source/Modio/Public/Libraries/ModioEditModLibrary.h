// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Types/ModioCommonTypes.h"
#include "Types/ModioEditModParams.h"

#include "ModioEditModLibrary.generated.h"

/**
 *
 */
UCLASS()
class MODIO_API UModioEditModLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "mod.io|Edit Mod Params")
	void SetName(UPARAM(ref) FModioEditModParams& In, FString Name);

	UFUNCTION(BlueprintCallable, Category = "mod.io|Edit Mod Params")
	void SetSummary(UPARAM(ref) FModioEditModParams& In, FString Summary);

	UFUNCTION(BlueprintCallable, Category = "mod.io|Edit Mod Params")
	void SetNamePath(UPARAM(ref) FModioEditModParams& In, FString NamePath);

	UFUNCTION(BlueprintCallable, Category = "mod.io|Edit Mod Params")
	void SetVisibility(UPARAM(ref) FModioEditModParams& In, bool Visibility);

	UFUNCTION(BlueprintCallable, Category = "mod.io|Edit Mod Params")
	void SetDescription(UPARAM(ref) FModioEditModParams& In, FString Description);

	UFUNCTION(BlueprintCallable, Category = "mod.io|Edit Mod Params")
	void SetHomepageURL(UPARAM(ref) FModioEditModParams& In, FString HomepageURL);

	UFUNCTION(BlueprintCallable, Category = "mod.io|Edit Mod Params")
	void SetMaturityFlags(UPARAM(ref) FModioEditModParams& In, EModioMaturityFlags MaturityFlags);

	UFUNCTION(BlueprintCallable, Category = "mod.io|Edit Mod Params")
	void SetMetadataBlob(UPARAM(ref) FModioEditModParams& In, FString MetadataBlob);
};
