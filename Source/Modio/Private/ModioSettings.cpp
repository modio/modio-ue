/* 
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *  
 *  This file is part of the mod.io UE4 Plugin.
 *  
 *  Distributed under the MIT License. (See accompanying file LICENSE or 
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *   
 */

#include "ModioSettings.h"

#include "Engine/Engine.h"
#include "ModioSubsystem.h"

UModioSettings::UModioSettings() : LogLevel(EModioLogLevel::Info) {}

#if WITH_EDITOR
void UModioSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	const FName PropertyName = PropertyChangedEvent.GetPropertyName();
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UModioSettings, LogLevel))
	{
		if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
		{
			Subsystem->SetLogLevel(LogLevel);
		}
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
