#pragma once

#include "UObject/Interface.h"
#include "Types/ModioCommonTypes.h"
#include "ModioUISubsystem.h"

#include "IModioUISubscriptionsChangedReceiver.generated.h"

UINTERFACE(BlueprintType)
class MODIOUI_API UModioUISubscriptionsChangedReceiver : public UInterface
{
	GENERATED_BODY()
};

class MODIOUI_API IModioUISubscriptionsChangedReceiver : public IInterface
{
	GENERATED_BODY()

	bool bRoutedSubscriptionsChanged = false;
	void SubscriptionsChangedHandler(FModioModID ModID, bool bNewSubscriptionState);
protected:
	template<typename ImplementingClass>
	void Register()
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			Subsystem->OnSubscriptionStatusChanged.AddUObject(
				Cast<ImplementingClass>(this), &IModioUISubscriptionsChangedReceiver::SubscriptionsChangedHandler);
		}
	}

	virtual void NativeOnSubscriptionsChanged(FModioModID ModID, bool bNewSubscriptionState);

	UFUNCTION(BlueprintImplementableEvent)
	void OnSubscriptionsChanged(FModioModID ModID, bool bNewSubscriptionState);
};
