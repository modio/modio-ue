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

#include "Templates/UniquePtr.h"

#include "ModioErrorCode.generated.h"

namespace Modio
{
	class ErrorCode;
}

/** @brief wrapper around Modio::ErrorCode */
USTRUCT(BlueprintType)
struct MODIO_API FModioErrorCode
{
	GENERATED_BODY()

	// This is to allow delegates to pass FModioErrorCode code, but I would have preferred it to be deleted
	FModioErrorCode();
	FModioErrorCode(const FModioErrorCode& Other);
	FModioErrorCode(Modio::ErrorCode ec);
	FModioErrorCode& operator=(const FModioErrorCode& ec);
	~FModioErrorCode();
	/** return true if this error code is a error */
	operator bool() const;

	/** Get the error code */
	int GetValue() const;
	Modio::ErrorCode GetRawErrorCode() const;

	/** Get a human readable message from the error code */
	FString GetErrorMessage() const;

private:
	TUniquePtr<Modio::ErrorCode> InternalError;
};

