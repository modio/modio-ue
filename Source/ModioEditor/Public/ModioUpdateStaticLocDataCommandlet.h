/*
 *  Copyright (C) 2025 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "Commandlets/Commandlet.h"
#include "CoreMinimal.h"

#include "ModioUpdateStaticLocDataCommandlet.generated.h"

/**
 * Updates the string table specified in the mod.io Plugin Settings page with static localization data for displaying
 * enums, error codes, etc Invoke like "UnrealEditor-cmd.exe" <path to .uproject file>
 * -Run=ModioEditor.ModioUpdateStaticLocData
 */
UCLASS()
class MODIOEDITOR_API UModioUpdateStaticLocDataCommandlet : public UCommandlet
{
	GENERATED_BODY()
#if WITH_EDITOR
	UModioUpdateStaticLocDataCommandlet();
	virtual int Main(const FString& Params) override;
#endif
};
