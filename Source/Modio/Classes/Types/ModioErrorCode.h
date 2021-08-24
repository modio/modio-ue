#pragma once

#include "ModioSDK.h"

// clang-format off
#include "ModioErrorCode.generated.h"
// clang-format on

/** @bried wrapper around the Modio::ErrorCode. This is a wrapper around the std::error_code */
USTRUCT(BlueprintType)
struct MODIO_API FModioErrorCode
{
	GENERATED_BODY()

	// This is to allow delegates to pass FModioErrorCode code, but I would have preferred it to be deleted
	FModioErrorCode() = default;
	FModioErrorCode(Modio::ErrorCode ec);

	/** return true if this error code is a error */
	FORCEINLINE operator bool() const
	{
		return Error.value() != 0;
	}

	/** Get the error code */
	FORCEINLINE int GetValue() const
	{
		return Error.value();
	}
	FORCEINLINE Modio::ErrorCode GetRawErrorCode() const
	{
		return Error;
	}

	/** Get a human readable message from the error code */
	FString GetMessage() const;

private:
	Modio::ErrorCode Error;
};

#pragma region ToUnreal implementation
FORCEINLINE FModioErrorCode ToUnreal(Modio::ErrorCode ec)
{
	return FModioErrorCode(ec);
}
#pragma endregion
