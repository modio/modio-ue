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

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Types/ModioErrorCode.h"

#include "ModioErrorCodeLibrary.generated.h"


UCLASS()
class MODIO_API UModioErrorCodeLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/**
	 * Checks if a error code contains a error
	 * @param Error -
	 * @return true if the error code is a error
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Error", meta = (CompactNodeTitle = "Error", BlueprintAutocast))
	static bool IsError(const FModioErrorCode& Error);

	/**
	 * Get underlying error code 
	 * @param Error -
	 * @return 0 if there is no error
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Error")
	static int32 GetValue(const FModioErrorCode& Error);

	/**
	 * Get the textual representation of the error
	 * @param Error -
	 * @return 
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Error")
	static FString GetMessage(const FModioErrorCode& Error);
};
