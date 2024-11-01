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

#include "CoreMinimal.h"
#include <Types/ModioModInfo.h>

#include "ModioEditModParamsObject.generated.h"

UCLASS()
class MODIOEDITOR_API UModioEditModParamsObject : public UObject
{
	GENERATED_BODY()

public:
	
	UModioEditModParamsObject() {}

	UPROPERTY(EditAnywhere, Category = "Modio Edit Mod Params")
	FString Name;

	UPROPERTY(EditAnywhere, Category = "Modio Edit Mod Params")
	FString Summary;

	UPROPERTY(EditAnywhere, Category = "Modio Edit Mod Params")
	FString NamePath;

	UPROPERTY(meta = (DeprecatedProperty, DeprecationMessage = "Deprecated as of 2023.6 release. Please use the <<Visibility>> instead."))
	bool bVisible_DEPRECATED;

	UPROPERTY(EditAnywhere, Category = "Modio Edit Mod Params")
	EModioObjectVisibilityFlags Visibility;

	UPROPERTY(EditAnywhere, Category = "Modio Edit Mod Params")
	FString Description;

	UPROPERTY(EditAnywhere, Category = "Modio Edit Mod Params")
	FString HomepageURL;

	UPROPERTY(EditAnywhere, Category = "Modio Edit Mod Params")
	EModioMaturityFlags MaturityFlags;

	UPROPERTY(EditAnywhere, Category = "Modio Edit Mod Params")
	FString MetadataBlob;

	UPROPERTY(EditAnywhere, Category = "Modio Edit Mod Params")
	FString LogoPath;
};
