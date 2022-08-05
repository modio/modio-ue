#pragma once

#include "Engine/Engine.h"
#include "ModioUISubsystem.h"
#include "Types/ModioCommonTypes.h"
#include "Types/ModioUser.h"
#include "UObject/Interface.h"
#include "ModioUISubsystem.h"

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

	template<typename ImplementingType>
	void Register()
	{
		if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
		{
			Subsystem->OnDisplayErrorNotification.AddUObject(
				Cast<ImplementingType>(this), &IModioUINotificationController::HandleDisplayErrorNotification);
			Subsystem->OnDisplayNotificationWidget.AddUObject(
				Cast<ImplementingType>(this), &IModioUINotificationController::HandleDisplayNotificationWidget);
		}
	}
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="ModioUINotificationController")
	void RegisterUserWidget(const TScriptInterface<IModioUINotificationController>& ControllerWidget);
	void RegisterUserWidget_Implementation(const TScriptInterface<IModioUINotificationController>& ControllerWidget)
	{
		if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
		{
			Subsystem->OnDisplayErrorNotification.AddUFunction(ControllerWidget.GetObject(),
															   FName("HandleDisplayErrorNotification"));
			Subsystem->OnDisplayNotificationWidget.AddUFunction(ControllerWidget.GetObject(), FName("HandleDisplayNotificationWidget"));
		}
	}

public:
	/// @brief Function to display an arbitrary notification widget that the caller has already configured
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="ModioUINotificationController")
	void DisplayNotification(const TScriptInterface<IModioUINotification>& Notification);

	/// @brief Function to display an error code notification widget
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="ModioUINotificationController")
	void DisplayErrorNotification(const FModioNotificationParams& Params);
};
