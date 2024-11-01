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
#include "ModioBrowseModFileObject.h"
#include "ModioBrowseModFileCollectionObject.generated.h"

UCLASS()
class MODIOEDITOR_API UModioBrowseModFileCollectionObject : public UObject
{
	GENERATED_BODY()

public:
	
	UModioBrowseModFileCollectionObject() {}

	UPROPERTY(EditAnywhere, Category = "Modio Browse Mod File Params")
	TArray<FModioBrowseModFileStruct> Items;
};
