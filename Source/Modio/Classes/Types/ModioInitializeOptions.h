#pragma once

#include "ModioSDK.h"
#include "Types/ModioCommonTypes.h"

// clang-format off
#include "ModioInitializeOptions.generated.h"
// clang-format on

USTRUCT(BlueprintType, meta = (HasNativeMake = "Modio.ModioCommonTypesLibrary.MakeInitializeOptions"))
struct FModioInitializeOptions
{
	GENERATED_BODY()

	/** Implicitly convert it to the underlying type */
	operator Modio::InitializeOptions() const;

	/** @brief The Mod.io-provided ID for the game */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io")
	FModioGameID GameID = FModioGameID::InvalidGameID();

	/** @brief The Mod.io-provided API key for your application or game */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io")
	FModioApiKey APIKey = FModioApiKey::InvalidAPIKey();

	/** @brief The mod.io environment you want to run the SDK on */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io")
	EModioEnvironment GameEnvironment = EModioEnvironment::Test;

	/** @brief The portal your title is running through */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io")
	EModioPortal PortalInUse = EModioPortal::None;
};
