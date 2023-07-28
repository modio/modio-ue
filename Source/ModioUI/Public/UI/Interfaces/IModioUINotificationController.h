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
class MODIOUI_API UModioUINotificationController : public UInterface
{
	GENERATED_BODY()
};

class MODIOUI_API IModioUINotificationController : public IInterface
{
	GENERATED_BODY()

protected:
	virtual void NativeDisplayNotification(const TScriptInterface<class IModioUINotification>& Notification) {};

	void DisplayNotification_Implementation(const TScriptInterface<IModioUINotification>& Notification)
	{
		NativeDisplayNotification(Notification);
	};

	virtual void NativeDisplayErrorNotification(const FModioNotificationParams& Params) {};
	void DisplayErrorNotification_Implementation(const FModioNotificationParams& Params)
	{
		NativeDisplayErrorNotification(Params);
	}
	virtual void NativeDisplayErrorNotificationManual(const FText& title, const FText& message, bool bIsError) {};
	void DisplayErrorNotificationManual_Implementation(const FText& title, const FText& message, bool bIsError)
	{
		NativeDisplayErrorNotificationManual(title, message, bIsError);
	}
	UFUNCTION()
	virtual void HandleDisplayErrorNotification(const FModioNotificationParams& Params)
	{
		Execute_DisplayErrorNotification(Cast<UObject>(this), Params);
	}
	UFUNCTION()
	virtual void HandleDisplayNotificationWidget(TScriptInterface<IModioUINotification>& Notification)
	{
		Execute_DisplayNotification(Cast<UObject>(this), Notification);
	}
	UFUNCTION()
	virtual void HandlDisplayErrorManual(const FText& title, const FText& message, bool bIsError)
	{
		Execute_DisplayErrorNotificationManual(Cast<UObject>(this), title, message, bIsError);
	}

	template<typename ImplementingType>
	void Register()
	{
		if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
		{
			Subsystem->OnDisplayErrorNotification.AddUObject(
				Cast<ImplementingType>(this), &IModioUINotificationController::HandleDisplayErrorNotification);
			Subsystem->OnDisplayNotificationWidget.AddUObject(
				Cast<ImplementingType>(this), &IModioUINotificationController::HandleDisplayNotificationWidget);
			Subsystem->OnDisplayErrorManualParams.AddUObject(
				Cast<ImplementingType>(this), &IModioUINotificationController::HandlDisplayErrorManual);
		}
	}
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ModioUINotificationController")
	void RegisterUserWidget(const TScriptInterface<IModioUINotificationController>& ControllerWidget);
	void RegisterUserWidget_Implementation(const TScriptInterface<IModioUINotificationController>& ControllerWidget)
	{
		if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
		{
			Subsystem->OnDisplayErrorNotification.AddUFunction(ControllerWidget.GetObject(),
															   FName("HandleDisplayErrorNotification"));
			Subsystem->OnDisplayNotificationWidget.AddUFunction(ControllerWidget.GetObject(),
																FName("HandleDisplayNotificationWidget"));
			Subsystem->OnDisplayErrorManualParams.AddUFunction(ControllerWidget.GetObject(),
																FName("HandleDisplayErrorManual"));

		}
	}

public:
	/// @brief Function to display an arbitrary notification widget that the caller has already configured
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ModioUINotificationController")
	void DisplayNotification(const TScriptInterface<IModioUINotification>& Notification);

	/// @brief Function to display an error code notification widget
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ModioUINotificationController")
	void DisplayErrorNotification(const FModioNotificationParams& Params);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ModioUINotificationController")
	void DisplayErrorNotificationManual(const FText& title, const FText& message, bool bIsError);
};
