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
	static void SetName(UPARAM(ref) FModioEditModParams& In, FString Name);

	UFUNCTION(BlueprintCallable, Category = "mod.io|Edit Mod Params")
	static void SetSummary(UPARAM(ref) FModioEditModParams& In, FString Summary);

	UFUNCTION(BlueprintCallable, Category = "mod.io|Edit Mod Params")
	static void SetNamePath(UPARAM(ref) FModioEditModParams& In, FString NamePath);

	UFUNCTION(BlueprintCallable, Category = "mod.io|Edit Mod Params")
	static void SetVisibility(UPARAM(ref) FModioEditModParams& In, bool Visibility);

	UFUNCTION(BlueprintCallable, Category = "mod.io|Edit Mod Params")
	static void SetDescription(UPARAM(ref) FModioEditModParams& In, FString Description);

	UFUNCTION(BlueprintCallable, Category = "mod.io|Edit Mod Params")
	static void SetHomepageURL(UPARAM(ref) FModioEditModParams& In, FString HomepageURL);

	UFUNCTION(BlueprintCallable, Category = "mod.io|Edit Mod Params")
	static void SetMaturityFlags(UPARAM(ref) FModioEditModParams& In, EModioMaturityFlags MaturityFlags);

	UFUNCTION(BlueprintCallable, Category = "mod.io|Edit Mod Params")
	static void SetMetadataBlob(UPARAM(ref) FModioEditModParams& In, FString MetadataBlob);
};
