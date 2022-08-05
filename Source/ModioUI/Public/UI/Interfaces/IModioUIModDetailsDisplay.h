#pragma once

#include "Engine/Engine.h"
#include "ModioUISubsystem.h"
#include "UObject/Interface.h"

#include "IModioUIModDetailsDisplay.generated.h"

UINTERFACE(BlueprintType)
class MODIOUI_API UModioUIModDetailsDisplay : public UInterface
{
	GENERATED_BODY()
};

class MODIOUI_API IModioUIModDetailsDisplay : public IInterface
{
	GENERATED_BODY()

protected:
	virtual void NativeDisplayModDetailsForId(const FModioModID& ModID) {};
	virtual void NativeDisplayModDetails(const TScriptInterface<class IModioModInfoUIDetails>& Details) {};
	void DisplayModDetails_Implementation(const TScriptInterface<IModioModInfoUIDetails>& Details)
	{
		NativeDisplayModDetails(Details);
	};
	void DisplayModDetailsForID_Implementation(const FModioModID& ModID)
	{
		NativeDisplayModDetailsForId(ModID);
	}

	UFUNCTION()
	virtual void HandleDisplayModDetails(TScriptInterface<IModioModInfoUIDetails>& Details)
	{
		Execute_DisplayModDetails(Cast<UObject>(this), Details);
	}

	UFUNCTION()
	virtual void HandleDisplayModDetailsForID(const FModioModID& ModID)
	{
		Execute_DisplayModDetailsForID(Cast<UObject>(this), ModID);
	}

	template<typename ImplementingType>
	void Register()
	{
		if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
		{
			Subsystem->OnDisplayModDetails.AddUObject(Cast<ImplementingType>(this),
															&IModioUIModDetailsDisplay::HandleDisplayModDetails);
			Subsystem->OnDisplayModDetailsForID.AddUObject(Cast<ImplementingType>(this),
															&IModioUIModDetailsDisplay::HandleDisplayModDetailsForID);
		}
	}
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="ModioUIModDetailsDisplay")
	void RegisterUserWidget(const TScriptInterface<IModioUIModDetailsDisplay>& DisplayWidget);
	void RegisterUserWidget_Implementation(const TScriptInterface<IModioUIModDetailsDisplay>& DisplayWidget)
	{
		if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
		{
			Subsystem->OnDisplayModDetails.AddUFunction(DisplayWidget.GetObject(), FName("HandleDisplayModDetails"));
			Subsystem->OnDisplayModDetailsForID.AddUFunction(DisplayWidget.GetObject(),
														FName("HandleDisplayModDetailsForID"));
		}
	}

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="ModioUIModDetailsDisplay")
	void DisplayModDetails(const TScriptInterface<IModioModInfoUIDetails>& Details);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="ModioUIModDetailsDisplay")
	void DisplayModDetailsForID(const FModioModID& ModID);
};
