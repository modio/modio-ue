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

#include "ModioUISubsystem.h"
#include "Types/ModioCommonTypes.h"
#include "Types/ModioUser.h"
#include "UObject/Interface.h"

#include "IModioUIUserChangedReceiver.generated.h"

UINTERFACE(BlueprintType)
class MODIOUICORE_API UModioUIUserChangedReceiver : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioUIUserChangedReceiver : public IInterface
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
