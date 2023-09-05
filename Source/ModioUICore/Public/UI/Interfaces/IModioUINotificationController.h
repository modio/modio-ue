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

#include "Engine/Engine.h"
#include "ModioUISubsystem.h"
#include "Types/ModioCommonTypes.h"
#include "Types/ModioUser.h"
#include "UObject/Interface.h"

#include "IModioUINotificationController.generated.h"

UINTERFACE(BlueprintType)
class MODIOUICORE_API UModioUINotificationController : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioUINotificationController : public IInterface
{
	GENERATED_BODY()

protected:
	virtual void NativeDisplayNotification(const TScriptInterface<class IModioUINotification>& Notification) {};

	void DisplayNotification_Implementation(const TScriptInterface<IModioUINotification>& Notification)
	{
		NativeDisplayNotification(Notification);
	}

	virtual void NativeDisplayNotificationParams(const FModioNotificationParams& Params) {};
	void DisplayNotificationParams_Implementation(const FModioNotificationParams& Params)
	{
		NativeDisplayNotificationParams(Params);
	}
	
	virtual void NativeDisplayNotificationManual(const FText& Title, const FText& Message, bool bIsError) {};
	void DisplayNotificationManual_Implementation(const FText& Title, const FText& Message, bool bIsError)
	{
		NativeDisplayNotificationManual(Title, Message, bIsError);
	}
	
	UFUNCTION()
	virtual void HandleDisplayNotificationParams(const FModioNotificationParams& Params)
	{
		Execute_DisplayNotificationParams(Cast<UObject>(this), Params);
	}
	
	UFUNCTION()
	virtual void HandleDisplayNotificationWidget(TScriptInterface<IModioUINotification>& Notification)
	{
		Execute_DisplayNotification(Cast<UObject>(this), Notification);
	}
	
	UFUNCTION()
	virtual void HandleDisplayManual(const FText& Title, const FText& Message, bool bIsError)
	{
		Execute_DisplayNotificationManual(Cast<UObject>(this), Title, Message, bIsError);
	}

	template<typename ImplementingType>
	void Register()
	{
		if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
		{
			Subsystem->OnDisplayNotificationParams.AddUObject(Cast<ImplementingType>(this), &IModioUINotificationController::HandleDisplayNotificationParams);
			Subsystem->OnDisplayNotificationWidget.AddUObject(Cast<ImplementingType>(this), &IModioUINotificationController::HandleDisplayNotificationWidget);
			Subsystem->OnDisplayManualParams.AddUObject(Cast<ImplementingType>(this), &IModioUINotificationController::HandleDisplayManual);
		}
	}
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ModioUINotificationController")
	void RegisterUserWidget(const TScriptInterface<IModioUINotificationController>& ControllerWidget);
	void RegisterUserWidget_Implementation(const TScriptInterface<IModioUINotificationController>& ControllerWidget)
	{
		if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
		{
			Subsystem->OnDisplayNotificationParams.AddUFunction(ControllerWidget.GetObject(), FName("HandleDisplayNotificationParams"));
			Subsystem->OnDisplayNotificationWidget.AddUFunction(ControllerWidget.GetObject(), FName("HandleDisplayNotificationWidget"));
			Subsystem->OnDisplayManualParams.AddUFunction(ControllerWidget.GetObject(), FName("HandleDisplayManual"));
		}
	}

public:
	/// @brief Function to display an arbitrary notification widget that the caller has already configured
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ModioUINotificationController")
	void DisplayNotification(const TScriptInterface<IModioUINotification>& Notification);

	/// @brief Function to display an error code notification widget
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ModioUINotificationController")
	void DisplayNotificationParams(const FModioNotificationParams& Params);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ModioUINotificationController")
	void DisplayNotificationManual(const FText& Title, const FText& Message, bool bIsError);
};
