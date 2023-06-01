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

#include "ModioCreateModParamsObject.generated.h"

UCLASS()
class MODIOEDITOR_API UModioCreateModParamsObject : public UObject
{
	GENERATED_BODY()

public:
	
	UModioCreateModParamsObject() {}

	UPROPERTY(EditAnywhere, Category = "Modio Create Mod Params")
	FString Name;

	UPROPERTY(EditAnywhere, Category = "Modio Create Mod Params")
	FString PathToLogoFile;

	UPROPERTY(EditAnywhere, Category = "Modio Create Mod Params")
	FString Summary;
};
