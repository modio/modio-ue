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
#include "Types/ModioOpenStoreResult.h"
#include "Types/ModioTokenPack.h"
#include "UObject/Interface.h"

#include "IModioPortalInterface.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FAuthTokenRequestedDelegate, const FString&, AuthToken);
DECLARE_DELEGATE_OneParam(FAuthTokenRequestedDelegateFast, const FString&);

DECLARE_DYNAMIC_DELEGATE_OneParam(FEntitlementParamsRequestedDelegate, const FModioEntitlementParams&,
								  EntitlementParams);
DECLARE_DELEGATE_OneParam(FEntitlementParamsRequestedDelegateFast, const FModioEntitlementParams&);

DECLARE_DYNAMIC_DELEGATE_OneParam(FSKUMappingsRequestedDelegate, const TArray<FModioTokenPack>&, SKUMappings);
DECLARE_DELEGATE_OneParam(FSKUMappingsRequestedDelegateFast, const TArray<FModioTokenPack>&);

DECLARE_DYNAMIC_DELEGATE_OneParam(FStoreClosedDelegate, bool, bResult);
DECLARE_DELEGATE_OneParam(FStoreClosedDelegateFast, bool);

// Delegate proxy objects are useful to wrap a native delegate inside a dynamic delegate. They allow the caller to bind
// additional arguments or use a lambda in their callback. The proxy object can be created and bound to a dynamic
// delegate with `CreateProxyDelegate` and will be added to the root set to prevent garbage collection. When the
// callback is executed, the proxy object will execute the native delegate and then remove itself from the root set and
// mark itself for garbage collection.

#define DELEGATE_PROXY_FUNCTIONS(Name)                                                    \
	static U##Name##Proxy* CreateProxy(const F##Name##DelegateFast& InCallback)           \
	{                                                                                     \
		U##Name##Proxy* Proxy = NewObject<U##Name##Proxy>();                              \
		Proxy->Callback = InCallback;                                                     \
		Proxy->AddToRoot();                                                               \
		return Proxy;                                                                     \
	}                                                                                     \
                                                                                          \
	static F##Name##Delegate CreateProxyDelegate(const F##Name##DelegateFast& InCallback) \
	{                                                                                     \
		U##Name##Proxy* Proxy = CreateProxy(InCallback);                                  \
		F##Name##Delegate DynamicDelegate;                                                \
		DynamicDelegate.BindDynamic(Proxy, &U##Name##Proxy::RequestComplete);             \
		return DynamicDelegate;                                                           \
	}                                                                                     \
                                                                                          \
	F##Name##DelegateFast Callback;

UCLASS()
class MODIO_API UAuthTokenRequestedProxy : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void RequestComplete(const FString& AuthToken)
	{
		Callback.ExecuteIfBound(AuthToken);
		RemoveFromRoot();
		ConditionalBeginDestroy();
	}

	DELEGATE_PROXY_FUNCTIONS(AuthTokenRequested);
};

UCLASS()
class MODIO_API UEntitlementParamsRequestedProxy : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void RequestComplete(const FModioEntitlementParams& EntitlementParams)
	{
		Callback.ExecuteIfBound(EntitlementParams);
		RemoveFromRoot();
		ConditionalBeginDestroy();
	}

	DELEGATE_PROXY_FUNCTIONS(EntitlementParamsRequested);
};

UCLASS()
class MODIO_API USKUMappingsRequestedProxy : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void RequestComplete(const TArray<FModioTokenPack>& SKUMappings)
	{
		Callback.ExecuteIfBound(SKUMappings);
		RemoveFromRoot();
		ConditionalBeginDestroy();
	}

	DELEGATE_PROXY_FUNCTIONS(SKUMappingsRequested);
};

UCLASS()
class MODIO_API UStoreClosedProxy : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void RequestComplete(bool bResult)
	{
		Callback.ExecuteIfBound(bResult);
		RemoveFromRoot();
		ConditionalBeginDestroy();
	}

	DELEGATE_PROXY_FUNCTIONS(StoreClosed);
};

UINTERFACE(BlueprintType, Category = "mod.io|PortalInterface",
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

	virtual void InitializePlatform_Implementation() {}

	virtual void RequestAuthToken_Implementation(const TMap<FString, FString>& Params,
										 const FAuthTokenRequestedDelegate& Callback)
	{
		Callback.ExecuteIfBound(TEXT(""));
	}

	virtual void RequestEntitlementParams_Implementation(const TMap<FString, FString>& Params,
												 const FEntitlementParamsRequestedDelegate& Callback)
	{
		Callback.ExecuteIfBound(FModioEntitlementParams());
	}

	virtual bool PrepareFilesystemToUsePath_Implementation(const FString& UgcPath)
	{
		return true;
	}

	virtual TMap<FString, FString> GetExtendedInitializationParams_Implementation()
	{
		return {};
	}

	virtual FString SanitizeFilePath_Implementation(FString& FilePath)
	{
		return FilePath;
	}

	virtual TMap<FString, FString> GetExtendedAuthParams_Implementation()
	{
		return {};
	}

	virtual void RequestSKUMappings_Implementation(const FSKUMappingsRequestedDelegate& Callback)
	{
		Callback.ExecuteIfBound({});
	}

	virtual EModioOpenStoreResult RequestOpenStore_Implementation(const FStoreClosedDelegate& Callback,
																  const FString& SKU)
	{
		Callback.ExecuteIfBound(false);
		return EModioOpenStoreResult::FailedUnknown;
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
	bool PrepareFilesystemToUsePath(const FString& UgcPath);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|PlatformInterface")
	TMap<FString, FString> GetExtendedInitializationParams();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|PlatformInterface")
	FString SanitizeFilePath(FString& FilePath);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|PlatformInterface")
	TMap<FString, FString> GetExtendedAuthParams();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|PlatformInterface")
	void RequestSKUMappings(const FSKUMappingsRequestedDelegate& Callback);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|PlatformInterface")
	EModioOpenStoreResult RequestOpenStore(const FStoreClosedDelegate& Callback, const FString& SKU);
};
