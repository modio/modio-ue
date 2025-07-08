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
#include "Types/ModioModInfo.h"

#include "ModioBrowseModFileObject.generated.h"

USTRUCT()
struct FModioBrowseModFileStruct
{
	GENERATED_BODY()

	FString Name;
	EModioModfilePlatform Platform;
	FString Version;
	FString Status;
	FModioModInfo ModInfo;

	TSharedPtr<FModioBrowseModFileStruct> ToSharedPtr()
	{
		TSharedPtr<FModioBrowseModFileStruct> SharedStruct = MakeShareable(new FModioBrowseModFileStruct());
		SharedStruct->Name = Name;
		SharedStruct->Platform = Platform;
		SharedStruct->Version = Version;
		SharedStruct->Status = Status;
		SharedStruct->ModInfo = ModInfo;
		return SharedStruct;
	}
};

UCLASS()
class MODIOEDITOR_API UModioBrowseModFileObject : public UObject
{
	GENERATED_BODY()
};
