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
#include <Types/ModioCommonTypes.h>

#include "ModioCreateNewModFileParamsObject.generated.h"

class SEditableTextBox;

UCLASS()
class MODIOEDITOR_API UModioCreateNewModFileParamsObject : public UObject
{
	GENERATED_BODY()

public:
	
	UModioCreateNewModFileParamsObject() {}

	UPROPERTY(EditAnywhere, Category = "Modio Create New Mod File Params")
	FString PathToModRootDirectory;

	UPROPERTY(EditAnywhere, Category = "Modio Create New Mod File Params")
	FString VersionString;
	
	UPROPERTY(EditAnywhere, Category = "Modio Create New Mod File Params")
	FString Changelog;

	UPROPERTY(EditAnywhere, Category = "Modio Create New Mod File Params")
	bool bSetAsActiveRelease;
	
	UPROPERTY(EditAnywhere, Category = "Modio Create New Mod File Params")
	FString MetadataBlob;

	UPROPERTY(EditAnywhere, Category = "Modio Create New Mod File Params")
	TArray<EModioModfilePlatform> ModfilePlatforms;
};
