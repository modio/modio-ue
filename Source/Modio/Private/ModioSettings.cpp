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
