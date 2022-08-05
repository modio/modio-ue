#pragma once

#include "UObject/Interface.h"
#include "Types/ModioCommonTypes.h"
#include "ModioUISubsystem.h"

#include "IModioUIModEnableWidget.generated.h"

UINTERFACE(BlueprintType)
class MODIOUI_API UModioUIModEnableWidget : public UInterface
{
	GENERATED_BODY()
};

class MODIOUI_API IModioUIModEnableWidget : public IInterface
{
	GENERATED_BODY()

	bool bRoutedModEnabledStateChanged = false;
	void ModEnabledStateChangedHandler(FModioModID ModID, bool bNewSubscriptionState);
protected:
	template<typename ImplementingClass>
	void Register()
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			//Subsystem->OnSubscriptionStatusChanged.AddUObject(
			//	Cast<ImplementingClass>(this), &IModioUIModEnableWidget::ModEnabledStateChangedHandler);
		}
	}

	virtual void NativeOnModEnabledStateChanged(FModioModID ModID, bool bNewSubscriptionState);

	UFUNCTION(BlueprintImplementableEvent)
	void OnModEnabledStateChanged(FModioModID ModID, bool bNewSubscriptionState);

    virtual bool QueryModEnabled(FModioModID ModID);

    virtual void RequestModEnabledState(FModioModID ModID, bool bNewStateIsEnabled);
};
