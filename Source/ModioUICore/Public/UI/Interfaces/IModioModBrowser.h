/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Types/ModioCommonTypes.h"
#include "Types/ModioErrorCode.h"
#include "Types/ModioFilterParams.h"
#include "Types/ModioModInfo.h"
#include "ModioSubsystem.h"
#include "Core/ModioFilterParamsUI.h"
#include "IModioModBrowser.generated.h"

// This class does not need to be modified.
UINTERFACE()
class MODIOUICORE_API UModioModBrowserInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MODIOUICORE_API IModioModBrowserInterface
{
	GENERATED_BODY()

public:
	/** Show the mod details widget for the given mod ID. Must be overriden by child classes */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IModioModBrowserInterface")
	void ShowDetailsForMod(FModioModID ID);

	/** Show the auth widget. Must be overriden by child classes */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IModioModBrowserInterface")
	void ShowUserAuth();

	/** Log out the current user. Must be overriden by child classes */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IModioModBrowserInterface")
	void LogOut();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IModioModBrowserInterface")
	void ShowSearchResults(const FModioModCategoryParams& FilterParams, bool bIsDefaultFilter);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IModioModBrowserInterface")
	void ShowReportMod(UObject* DialogDataSource);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IModioModBrowserInterface")
	void ShowModUnsubscribeDialog(UObject* DialogDataSource);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IModioModBrowserInterface")
	void RequestExternalAuthentication(EModioAuthenticationProvider Provider);

	virtual void RequestExternalAuthenticationNative(EModioAuthenticationProvider Provider, FOnErrorOnlyDelegateFast DedicatedCallback){}

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IModioModBrowserInterface")
	bool IsDownloadDrawerOpen();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IModioModBrowserInterface")
	bool GetIsCollectionModDisableUIEnabled();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IModioModBrowserInterface")
	void ShowDialog(UObject* DialogDataSource);
};
