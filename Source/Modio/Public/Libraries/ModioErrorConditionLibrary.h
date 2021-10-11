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
#include "ModioErrorCondition.h"

#include "ModioErrorConditionLibrary.generated.h"


UCLASS()
class MODIO_API UModioErrorConditionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * @brief Checks if the passed-in ErrorCode matches the specified error condition
	 * @param ErrorCode The code to check
	 * @param Condition The error condition to check against
	 * @return true if the code matches the condition
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|Error Handling")
	static bool ErrorCodeMatches(FModioErrorCode ErrorCode, EModioErrorCondition Condition);
};