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

#include "ModioBrowseModsObject.generated.h"

UCLASS()
class MODIOEDITOR_API UModioBrowseModsObject : public UObject
{
	GENERATED_BODY()

public:
	
	UModioBrowseModsObject() {}

	UPROPERTY(EditAnywhere, Category = "Browse Mods Params")
    TArray<FModioModInfo> Items;

	TSharedPtr<FModioModInfo> SelectedItem;
};
