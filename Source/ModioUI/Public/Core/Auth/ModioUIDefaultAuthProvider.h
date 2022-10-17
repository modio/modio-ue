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

#include "Containers/Array.h"
#include "Types/ModioAuthenticationParams.h"
#include "UI/Interfaces/IModioUIAuthenticationDataProvider.h"
#include "UObject/Object.h"

#include "ModioUIDefaultAuthProvider.generated.h"

UCLASS()
class MODIOUI_API UModioUIDefaultAuthProvider : public UObject, public IModioUIAuthenticationDataProvider
{
	GENERATED_BODY()
protected:
	TArray<FModioUIAuthenticationProviderInfo> NativeGetAuthenticationTypes() override
	{
		// Default implementation, the built-in default provider only supports email auth
		return {};
	}

	FModioAuthenticationParams NativeGetAuthenticationParams(EModioAuthenticationProvider) override
	{
		// Default implementation, the built-in default provider only supports email auth
		return {};
	}

	bool NativeShouldOfferEmailAuthentication() override
	{
		return true;
	}
};