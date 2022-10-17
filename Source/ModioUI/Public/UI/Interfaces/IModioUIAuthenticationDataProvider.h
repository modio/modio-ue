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

#include "Types/ModioAuthenticationParams.h"
#include "UObject/Interface.h"

#include "IModioUIAuthenticationDataProvider.generated.h"

/**
* Strong UI type struct to store the authentication provider information
**/
USTRUCT(BlueprintType)
struct MODIOUI_API FModioUIAuthenticationProviderInfo
{
	GENERATED_BODY()
	
	/**
	* Static method to retrieve the information of an email authentication provider information
	* @return UI Authentication Provider Information instance
	**/
	static FModioUIAuthenticationProviderInfo EmailAuth();

	/**
	* Boolean property to enable encourage email verification as the external auth method.
	* Not exposed to blueprint because it should only be created if the data provider indicates email auth should be
	* offered. False by default
	**/
	UPROPERTY()
	bool bIsEmailAuthentication = false;

	/**
	* Stored property for the ProviderID, defined by EModioAuthenticationProvider enum 
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "!bIsEmailAuthentication", EditConditionHides),
			  Category = "ModioUIAuthenticationProviderInfo")
	EModioAuthenticationProvider ProviderID;

	/**
	* Stored property for the provider label, a text version of the EModioAuthenticationProvider enum 
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ModioUIAuthenticationProviderInfo")
	FText ProviderUILabel;
};

/**
* Modio UI element with the basic functionality to interact with an authentication provider,
* subclass of UInterface
**/
UINTERFACE(BlueprintType)
class MODIOUI_API UModioUIAuthenticationDataProvider : public UInterface
{
	GENERATED_BODY()
};

/**
* Modio UI element with the basic functionality to interact with an authentication provider,
* subclass of IInterface
**/
class MODIOUI_API IModioUIAuthenticationDataProvider : public IInterface
{
	GENERATED_BODY()
protected:
	virtual TArray<FModioUIAuthenticationProviderInfo> NativeGetAuthenticationTypes()
		PURE_VIRTUAL(IModioUIAuthenticationDataProvider::NativeGetAuthenticationTypes, return {};);

	virtual FModioAuthenticationParams NativeGetAuthenticationParams(EModioAuthenticationProvider)
		PURE_VIRTUAL(IModioUIAuthenticationDataProvider::NativeGetAuthenticationParams, return {};);

	virtual bool NativeShouldOfferEmailAuthentication()
	{
		return false;
	}

public:
	/// @brief Allows the consuming title to inform the UI which types of external authentication the UI should offer to
	/// the user
	/// @returns An array of EModioAuthenticationProvider values, one for each external authentication to support
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ModioUIAuthenticationDataProvider")
	TArray<FModioUIAuthenticationProviderInfo> GetAuthenticationTypes();

	// GetAuthenticationTypes_Implementation
	TArray<FModioUIAuthenticationProviderInfo> GetAuthenticationTypes_Implementation()
	{
		return NativeGetAuthenticationTypes();
	}

	/// @brief Callback so the consuming application can populate the relevant OAuth token
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ModioUIAuthenticationDataProvider")
	FModioAuthenticationParams GetAuthenticationParams(EModioAuthenticationProvider Provider);

	// GetAuthenticationParams_Implementation
	FModioAuthenticationParams GetAuthenticationParams_Implementation(EModioAuthenticationProvider Provider)
	{
		return NativeGetAuthenticationParams(Provider);
	}

	/**
	* Backend configuration that email should be provided as the authentication method
	* @return True if email authentication should be offerred as priority, false otherwise
	**/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ModioUIAuthenticationDataProvider")
	bool ShouldOfferEmailAuthentication();

	// ShouldOfferEmailAuthentication_Implementation
	bool ShouldOfferEmailAuthentication_Implementation()
	{
		return NativeShouldOfferEmailAuthentication();
	}
};