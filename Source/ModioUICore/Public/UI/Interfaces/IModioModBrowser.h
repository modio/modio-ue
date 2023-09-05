// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Types/ModioCommonTypes.h"
#include "Types/ModioErrorCode.h"
#include "Types/ModioFilterParams.h"
#include "Types/ModioModInfo.h"
#include "ModioSubsystem.h"
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
	void ShowSearchResults(const FModioFilterParams& FilterParams);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IModioModBrowserInterface")
	void ShowReportMod(UObject* DialogDataSource);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IModioModBrowserInterface")
	void ShowModUnsubscribeDialog(UObject* DialogDataSource);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IModioModBrowserInterface")
	void ShowDialog(FModioModInfo ModInfo);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IModioModBrowserInterface")
	void RequestExternalAuthentication(EModioAuthenticationProvider Provider);

	virtual void RequestExternalAuthenticationNative(EModioAuthenticationProvider Provider, FOnErrorOnlyDelegateFast DedicatedCallback){}

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IModioModBrowserInterface")
	bool IsDownloadDrawerOpen();
};
