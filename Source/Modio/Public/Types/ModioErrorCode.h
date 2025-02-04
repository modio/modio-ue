/* 
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *  
 *  This file is part of the mod.io UE Plugin.
 *  
 *  Distributed under the MIT License. (See accompanying file LICENSE or 
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *   
 */

#pragma once

#include <cstdint>
#include "Templates/UniquePtr.h"

#include "ModioErrorCode.generated.h"

namespace Modio
{
	class ErrorCode;
}

/** 
 * @docpublic
 * @brief Wrapper around `Modio::ErrorCode` 
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioErrorCode
{
	GENERATED_BODY()

	// This is to allow delegates to pass FModioErrorCode code, but I would have preferred it to be deleted
	FModioErrorCode();
	FModioErrorCode(const FModioErrorCode& Other);
	FModioErrorCode(Modio::ErrorCode ec);
	FModioErrorCode(int32 Value, int32 CategoryID);
	FModioErrorCode& operator=(const FModioErrorCode& ec);
	~FModioErrorCode();

	/** @docpublic
	 * @brief Return true if this error code is an error
	 * @return true if this error code represents an error
	 */
	operator bool() const;

	/** @docpublic
	 * @brief Get the error code
	 * @return integer value of the error code
	 */
	int GetValue() const;

	/** @docpublic
	 * @brief Get the category string of the error
	 * @return string representing the error's category
	 */
	FString GetCategoryString() const;

	/** @docpublic
	 * @brief Get the raw error code
	 * @return Modio::ErrorCode representing the raw error
	 */
	Modio::ErrorCode GetRawErrorCode() const;

	/** @docpublic
	 * @brief Get a human-readable message from the error code
	 * @return string with a message describing the error
	 */
	FString GetErrorMessage() const;

	/** @docpublic
	 * @brief Static method to return a system error instance
	 * @return FModioErrorCode instance representing a system error
	 */
	static FModioErrorCode SystemError();

	/** @docpublic
	 * @brief Get the category ID associated with the error
	 * @return category ID as an integer
	 */
	int32 GetCategoryID();

private:
	TUniquePtr<Modio::ErrorCode> InternalError;
};
