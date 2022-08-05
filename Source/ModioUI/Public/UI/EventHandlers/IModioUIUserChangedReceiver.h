#pragma once

#include "Types/ModioCommonTypes.h"
#include "Types/ModioUser.h"
#include "UObject/Interface.h"
#include "ModioUISubsystem.h"

#include "IModioUIUserChangedReceiver.generated.h"

UINTERFACE(BlueprintType)
class MODIOUI_API UModioUIUserChangedReceiver : public UInterface
{
	GENERATED_BODY()
};

class MODIOUI_API IModioUIUserChangedReceiver : public IInterface
{
	GENERATED_BODY()

	bool bRoutedUserChanged = false;
	void UserChangedHandler(TOptional<FModioUser> NewUser);

protected:
	template<typename ImplementingClass>
	void Register()
	{
		
		if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
		{
			Subsystem->OnUserChanged.AddUObject(Cast<ImplementingClass>(this),
												&IModioUIUserChangedReceiver::UserChangedHandler);
		}
		if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
		{
			UserChangedHandler(Subsystem->QueryUserProfile());
		}
	}

	virtual void NativeUserChanged(TOptional<FModioUser> NewUser);

	UFUNCTION(BlueprintImplementableEvent)
	void OnUserChanged(FModioOptionalUser NewUser);
};
