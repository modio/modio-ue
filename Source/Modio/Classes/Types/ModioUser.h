#pragma once

#include "Types/ModioCommonTypes.h"

// clang-format off
#include "ModioUser.generated.h"
// clang-format on

namespace Modio
{
	struct User;
}

/** @brief Object representing a mod.io user profile **/
USTRUCT(BlueprintType)
struct MODIO_API FModioUser
{
	GENERATED_BODY()

	FModioUser() = default;
	FModioUser(const Modio::User& User);

	/** @brief Unique id for the user */
	UPROPERTY(BlueprintReadOnly,Category="User")
	FModioUserID UserId;

	/** @brief Username of the user */
	UPROPERTY(BlueprintReadOnly,Category="User")
	FString Username;

	/** @brief Unix timestamp the user was last online */
	UPROPERTY(BlueprintReadOnly,Category="User")
	FDateTime DateOnline;

	/** @brief URL of the user's mod.io profile */
	UPROPERTY(BlueprintReadOnly,Category="User")
	FString ProfileUrl;
};

USTRUCT(BlueprintType)
struct MODIO_API FModioOptionalUser
{
	GENERATED_BODY()

	FModioOptionalUser() = default;
	FModioOptionalUser(TOptional<FModioUser>&& ModInfoList);

	TOptional<FModioUser> Internal;
};

#pragma region ToUnreal implementation
FORCEINLINE FModioUser ToUnreal(const Modio::User& Value)
{
	return FModioUser(Value);
}
#pragma endregion
