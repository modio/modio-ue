#pragma once

#include "ModioUISubsystem.h"
#include "UObject/Interface.h"

#include "IModioUIAuthenticationChangedReceiver.generated.h"

UINTERFACE(BlueprintType)
class MODIOUI_API UModioUIAuthenticationChangedReceiver : public UInterface
{
	GENERATED_BODY()
};

class MODIOUI_API IModioUIAuthenticationChangedReceiver : public IInterface
{
	GENERATED_BODY()

	bool bRoutedAuthenticationChanged = false;

	void UserAuthenticationChangedHandler(TOptional<FModioUser> User);

protected:
	template<typename ImplementingClass>
	void Register()
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			Subsystem->OnUserChanged.AddUObject(
				Cast<ImplementingClass>(this), &IModioUIAuthenticationChangedReceiver::UserAuthenticationChangedHandler);

			// Update with the "current" state of user authentication
			UserAuthenticationChangedHandler(GEngine->GetEngineSubsystem<UModioSubsystem>()->QueryUserProfile());
		}
	}

	virtual void NativeOnAuthenticationChanged(TOptional<FModioUser> User);

	UFUNCTION(BlueprintImplementableEvent)
	void OnAuthenticationChanged(FModioOptionalUser User);
};
