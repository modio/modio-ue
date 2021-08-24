#pragma once

#include "ModioSDK.h"

#include "Internal/ModioPrivateDefines.h"
#include "Containers/UnrealString.h"
#include "Misc/Optional.h"

#include "ModioAuthenticationParams.generated.h"

/** @brief Simple struct to encapsulate data passed to external authentication systems */
UENUM(BlueprintType)
enum class EModioAuthenticationProvider : uint8
{
	XboxLive,
	Steam,
	GoG,
	Itch,
	Switch,
	Discord
};

/** @brief Simple struct to encapsulate data passed to external authentication systems */
USTRUCT(BlueprintType)
struct FModioAuthenticationParams
{
	GENERATED_BODY();

	operator Modio::AuthenticationParams() const;
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly)
	FString AuthToken;
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly)
	FString UserEmail;
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly)
	bool bUserHasAcceptedTerms = false;
};

#pragma region ToModio implementation
MODIO_BEGIN_CONVERT_SWITCHES
FORCEINLINE Modio::AuthenticationProvider ToModio(EModioAuthenticationProvider Provider)
{
	switch (Provider)
	{
		case EModioAuthenticationProvider::XboxLive:
			return Modio::AuthenticationProvider::XboxLive;
		case EModioAuthenticationProvider::Steam:
			return Modio::AuthenticationProvider::Steam;
		case EModioAuthenticationProvider::GoG:
			return Modio::AuthenticationProvider::GoG;
		case EModioAuthenticationProvider::Itch:
			return Modio::AuthenticationProvider::Itch;
		case EModioAuthenticationProvider::Switch:
			return Modio::AuthenticationProvider::Switch;
		case EModioAuthenticationProvider::Discord:
			return Modio::AuthenticationProvider::Discord;
	}

	return Modio::AuthenticationProvider::Steam;
}
MODIO_END_CONVERT_SWITCHES
#pragma endregion
