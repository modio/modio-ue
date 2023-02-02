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

#include "Types/ModioTerms.h"
#include "UObject/Interface.h"
#include "UI/Interfaces/IModioUIAuthenticationDataProvider.h"

#include "IModioAuthenticationContextUIDetails.generated.h"

/**
 * Implementation details for ModioAuthenticationContextUI
 */
UINTERFACE(BlueprintType)
class MODIOUI_API UModioAuthenticationContextUIDetails : public UInterface
{
	GENERATED_BODY()
};

class MODIOUI_API IModioAuthenticationContextUIDetails : public IInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IModioAuthenticationContextUIDetails")
	FModioTerms GetTerms();
	virtual FModioTerms GetTerms_Implementation()
	{
		return FModioTerms();
	};

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IModioAuthenticationContextUIDetails")
	FModioUIAuthenticationProviderInfo GetProviderInfo();
	virtual FModioUIAuthenticationProviderInfo GetProviderInfo_Implementation()
	{
		return FModioUIAuthenticationProviderInfo();
	}
};