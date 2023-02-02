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
#include "Types/ModioTerms.h"
#include "UI/Interfaces/IModioAuthenticationContextUIDetails.h"
#include "UI/Interfaces/IModioUIAuthenticationDataProvider.h"
#include "UObject/NoExportTypes.h"

#include "ModioAuthenticationContextUI.generated.h"

/**
 * Wrapper around a FModioTerms and FModioUIAuthenticationProviderInfo because UMG widgets expect list items to be UObject-based
 **/
UCLASS(BlueprintType)
class MODIOUI_API UModioAuthenticationContextUI : public UObject, public IModioAuthenticationContextUIDetails
{
	GENERATED_BODY()
public:
	virtual FModioTerms GetTerms_Implementation() override
	{
		return Terms;
	}

	virtual FModioUIAuthenticationProviderInfo GetProviderInfo_Implementation() override
	{
		return ProviderInfo;
	}

	/**
	 * Stored property to the FModioTerms in this class
	 **/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ModioAuthenticationContextUI")
	FModioTerms Terms;

	/**
	 * Stored property to the FModioUIAuthenticationProviderInfo in this class
	 **/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ModioAuthenticationContextUI")
	FModioUIAuthenticationProviderInfo ProviderInfo;
};
