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
#include "UObject/Interface.h"
#include "ModioUISubsystem.h"
#include "ModioUI4Subsystem.h"
#include "UI/ModioUIStatics.h"
#include "IModioUIActionHandler.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UModioUIActionHandler : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MODIOUI_API IModioUIActionHandler
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ModioUIActionHandler")
	void OnMenuAction(EMenuAction Action, UObject* OptionalData);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ModioUIActionHandler")
	void OnErrorCodeReceived(FModioErrorCode ec);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ModioUIActionHandler")
	bool OnSearchResultParamsReceived(FModioFilterParams params);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ModioUIActionHandler")
	void OnModIDReceived(const FModioModID& ID);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ModioUIActionHandler")
	void OnCursorVisibilityChanged(bool bNewVisibility);

protected:
	void OnMenuAction_Implementation(EMenuAction Action, UObject* OptionalData)
	{
		NativeOnMenuAction(Action, OptionalData);
	}

	void OnErrorCodeReceived_Implementation(FModioErrorCode ec)
	{
		NativeOnErrorCodeReceived(ec);
	}

	bool OnSearchResultParamsReceived_Implementation(FModioFilterParams params)
	{
		return NativeOnSearchResultParamsReceived(params);
	}

	void OnModIDReceived_Implementation(const FModioModID& ID)
	{
		NativeOnModIDReceived(ID);
	}

	
	void OnCursorVisibilityChanged_Implementation(bool bNewVisibility)
	{
		NativeOnCursorVisibilityChanged(bNewVisibility);
	}

	template<typename ImplementingType>
	void Register()
	{
		if (UModioUI4Subsystem* Subsystem4 = GEngine->GetEngineSubsystem<UModioUI4Subsystem>())
		{
			Subsystem4->OnMenuAction.AddUObject(Cast<ImplementingType>(this),
												&IModioUIActionHandler::HandleOnMenuAction);
			Subsystem4->OnCursorVisibilityChanged.AddUObject(Cast<ImplementingType>(this),
															&IModioUIActionHandler::HandleOnCursorVisibilityChanged);
		}
		if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
		{
			//Subsystem->OnDisplayErrorDialog.AddUObject(Cast<ImplementingType>(this),
			//										   &IModioUIActionHandler::HandleOnErrorCodeReceived);
			Subsystem->OnDisplayModDetailsForID.AddUObject(Cast<ImplementingType>(this),
														   &IModioUIActionHandler::HandleOnModIDReceived);
			Subsystem->OnDisplaySearchResults.BindDynamic(Cast<ImplementingType>(this),
														   &IModioUIActionHandler::HandleSearchResultParamsReceived);
		}
	}
	UFUNCTION()
	virtual void HandleOnMenuAction(EMenuAction Action, UObject* OptionalData)
	{
		Execute_OnMenuAction(Cast<UObject>(this), Action, OptionalData);
	}

	UFUNCTION()
	virtual void HandleOnErrorCodeReceived(FModioErrorCode ec)
	{
		Execute_OnErrorCodeReceived(Cast<UObject>(this), ec);
	}

	UFUNCTION()
	virtual bool HandleSearchResultParamsReceived(FModioFilterParams params)
	{
		return Execute_OnSearchResultParamsReceived(Cast<UObject>(this), params);
	}

	UFUNCTION()
	virtual void HandleOnModIDReceived(const FModioModID& ID)
	{
		Execute_OnModIDReceived(Cast<UObject>(this), ID);
	}

	UFUNCTION()
	virtual void HandleOnCursorVisibilityChanged(bool bNewVisibility)
	{
		Execute_OnCursorVisibilityChanged(Cast<UObject>(this), bNewVisibility);
	}

	virtual void NativeOnMenuAction(EMenuAction Action, UObject* OptionalData) {};
	virtual void NativeOnErrorCodeReceived(FModioErrorCode ec) {};
	virtual bool NativeOnSearchResultParamsReceived(FModioFilterParams params)	{return false;}
	virtual void NativeOnModIDReceived(const FModioModID& ID) {};
	virtual void NativeOnCursorVisibilityChanged(bool bNewVisibility) {};
};