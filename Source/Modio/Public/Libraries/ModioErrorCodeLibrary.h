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

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Types/ModioErrorCode.h"

#include "ModioErrorCodeLibrary.generated.h"

/**
 * @docpublic
 * @brief Utility functions for working with error codes
 */
UCLASS()
class MODIO_API UModioErrorCodeLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * @docpublic
	 * @brief Checks if an error code contains a error
	 * @return true if the error code is an error
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Error", meta = (CompactNodeTitle = "Error", BlueprintAutocast))
	static bool IsError(const FModioErrorCode& Error);

	/**
	 * @docpublic
	 * @brief Checks if an error code contains a error
	 * @return true if the error code is an error
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|Utilities|Error",
		meta = (DisplayName = "IsError", ExpandBoolAsExecs = "ReturnValue"))
	static bool IsErrorAsExec(const FModioErrorCode& Error);

	/**
	 * @docpublic
	 * @brief Get underlying error code for an FModioErrorCode.
	 * @return The underlying error code.  0 represents no error.
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Error")
	static int32 GetValue(const FModioErrorCode& Error);

	/**
	 * @docpublic
	 * @brief Get the textual representation of the error
	 * @return An `FString` message describing the error
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Error")
	static FString GetMessage(const FModioErrorCode& Error);

	/**
	 * @docpublic
	 * @brief Helper method to reconstruct a mod.io error passed via code that cannot reference mod.io types
	 * @param Value The numeric value of the code
	 * @param Category The category ID (populated by native code)
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Error")
	static FModioErrorCode ReconstructError(int32 Value, int32 Category);
};
