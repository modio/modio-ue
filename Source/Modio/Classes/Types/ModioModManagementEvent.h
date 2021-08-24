#pragma once

#include "Internal/ModioPrivateDefines.h"
#include "Types/ModioCommonTypes.h"
#include "Types/ModioErrorCode.h"

// clang-format off
#include "ModioModManagementEvent.generated.h"
// clang-format on

/** @brief What type of event occurred */
UENUM(BlueprintType)
enum class EModioModManagementEventType : uint8
{
	Installed, /** Mod installation to local storage */
	Uninstalled, /** Mod uninstallation from local storage*/
	Updated /** Mod local installation updated to latest version*/
};

/** @brief Simple struct representing the outcome of a mod management operation */
USTRUCT(BlueprintType)
struct MODIO_API FModioModManagementEvent
{
	GENERATED_BODY()

	// This is to allow delegates to pass FModioModManagementEvent code, but I would have preferred it to be deleted
	FModioModManagementEvent() = default;
	FModioModManagementEvent(const Modio::ModManagementEvent& Event);

	/** @brief ID for the mod that the event occurred on */
	UPROPERTY(BlueprintReadOnly,Category="ModManagementEvent")
	FModioModID ID;

	/** @brief What type of event occurred */
	UPROPERTY(BlueprintReadOnly,Category="ModManagementEvent")
	EModioModManagementEventType Event;

	/** @brief Empty if operation completed successfully, truthy/contains error code if operation failed */
	UPROPERTY(BlueprintReadOnly,Category="ModManagementEvent")
	FModioErrorCode Status;
};

#pragma region ToUnreal implementation
FORCEINLINE FModioModManagementEvent ToUnreal(const Modio::ModManagementEvent& Event)
{
	return FModioModManagementEvent(Event);
}

MODIO_BEGIN_CONVERT_SWITCHES
FORCEINLINE EModioModManagementEventType ToUnreal(Modio::ModManagementEvent::EventType Event)
{
	switch (Event)
	{
		case Modio::ModManagementEvent::EventType::Installed:
			return EModioModManagementEventType::Installed;
		case Modio::ModManagementEvent::EventType::Uninstalled:
			return EModioModManagementEventType::Uninstalled;
		case Modio::ModManagementEvent::EventType::Updated:
			return EModioModManagementEventType::Updated;
	}

	checkf(false, TEXT("Missed a case in ToModio(EModioEnvironment Environment)"));
	return EModioModManagementEventType::Installed;
}
MODIO_END_CONVERT_SWITCHES

#pragma endregion
