/*
 *  Copyright (C) 2026 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "Types/ModioCommonTypes.h"
#include "UObject/Interface.h"

#include "IModioPortalInterface.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FAuthTokenRequestedDelegate, const FString&, AuthToken);
DECLARE_DELEGATE_OneParam(FAuthTokenRequestedDelegateFast, const FString&);

DECLARE_DYNAMIC_DELEGATE_OneParam(FEntitlementParamsRequestedDelegate, const FModioEntitlementParams&, EntitlementParams);
DECLARE_DELEGATE_OneParam(FEntitlementParamsRequestedDelegateFast, const FModioEntitlementParams&);

UINTERFACE(
	BlueprintType, Category = "mod.io|PortalInterface",
	Meta = (DocGroup = "Portal Interfaces",
			Comment = "@brief Interface for objects that provide portal/platform specific implementations"))
class MODIO_API UModioPortalInterface : public UInterface
{
	GENERATED_BODY()
};

class MODIO_API IModioPortalInterface : public IInterface
{
	GENERATED_BODY()

protected:
	virtual void NativeInitializePlatform() {}

	virtual void NativeRequestAuthToken(const TMap<FString, FString>& Params,
												const FAuthTokenRequestedDelegateFast& Callback)
	{
		Callback.ExecuteIfBound(TEXT(""));
	}

	virtual void NativeRequestEntitlementParams(const TMap<FString, FString>& Params,
												const FEntitlementParamsRequestedDelegateFast& Callback)
	{
		Callback.ExecuteIfBound(FModioEntitlementParams());
	}

	virtual FString NativeGetAuthKey()
	{
		return TEXT("");
	}

	virtual bool NativePrepareFilesystemToUsePath(const FString& UgcPath)
	{
		return true;
	}

	virtual TMap<FString, FString> NativeGetExtendedInitializationParams()
	{
		return {};
	}

	virtual FString NativeSanitizeFilePath(FString& FilePath)
	{
		return FilePath;
	}

	virtual TMap<FString, FString> NativeGetExtendedAuthParams()
	{
		return {};
	}

	void InitializePlatform_Implementation()
	{
		return NativeInitializePlatform();
	}

	void RequestAuthToken_Implementation(const TMap<FString, FString>& Params,
												 const FAuthTokenRequestedDelegate& Callback)
	{
		NativeRequestAuthToken(
			Params, FAuthTokenRequestedDelegateFast::CreateLambda(
						[Callback](const FString& AuthToken) { Callback.ExecuteIfBound(AuthToken); }));
	}

	void RequestEntitlementParams_Implementation(const TMap<FString, FString>& Params,
													const FEntitlementParamsRequestedDelegate& Callback)
	{
		NativeRequestEntitlementParams(
			Params, FEntitlementParamsRequestedDelegateFast::CreateLambda(
						[Callback](const FModioEntitlementParams& EntitlementParams) {
							Callback.ExecuteIfBound(EntitlementParams);
												   }));
	}

	FString GetAuthKey_Implementation()
	{
		return NativeGetAuthKey();
	}

	bool PrepareFilesystemToUsePath_Implementation(const FString& UgcPath)
	{
		return NativePrepareFilesystemToUsePath(UgcPath);
	}

	TMap<FString, FString> GetExtendedInitializationParams_Implementation()
	{
		return NativeGetExtendedInitializationParams();
	}

	FString SanitizeFilePath_Implementation(FString& FilePath)
	{
		return NativeSanitizeFilePath(FilePath);
	}

	TMap<FString, FString> GetExtendedAuthParams_Implementation()
	{
		return NativeGetExtendedAuthParams();
	}

public:
	/**
	 * For performing basic platform initialization, such as preparing the filesystem or
	 * gathering user tokens
	 **/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|PlatformInterface")
	void InitializePlatform();

	/**
	 * Requests the auth token for the specific platform
	 * @param Params can provide additional parameters for login
	 * @param Callback will be called with the auth token
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|PlatformInterface")
	void RequestAuthToken(const TMap<FString, FString>& Params, const FAuthTokenRequestedDelegate& Callback);

	/**
	 * Requests the entitlement parameters for the specific platform
	 * @param Params can provide additional parameters for login
	 * @param Callback will be called with the entitlement parameters
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|PlatformInterface")
	void RequestEntitlementParams(const TMap<FString, FString>& Params,
								  const FEntitlementParamsRequestedDelegate& Callback);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|PlatformInterface")
	FString GetAuthKey();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|PlatformInterface")
	bool PrepareFilesystemToUsePath(const FString& UgcPath);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|PlatformInterface")
	TMap<FString, FString> GetExtendedInitializationParams();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|PlatformInterface")
	FString SanitizeFilePath(FString& FilePath);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|PlatformInterface")
	TMap<FString, FString> GetExtendedAuthParams();
};
