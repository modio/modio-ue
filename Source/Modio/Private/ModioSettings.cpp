/* 
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *  
 *  This file is part of the mod.io UE Plugin.
 *  
 *  Distributed under the MIT License. (See accompanying file LICENSE or 
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *   
 */

#include "ModioSettings.h"

#include "Engine/Engine.h"
#include "ModioSubsystem.h"

#if WITH_EDITOR
#include "UnrealEdMisc.h"
#endif

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioSettings)

UModioSettings::UModioSettings()
	: LogLevel(EModioLogLevel::Info),
	  LocalizationStringTable("/Modio/Data/ST_ModioStaticLocData.ST_ModioStaticLocData")
{}

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
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(UModioSettings, bUseBackgroundThread))
	{
		const bool bWarn = true;
		FUnrealEdMisc::Get().RestartEditor(bWarn);
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
