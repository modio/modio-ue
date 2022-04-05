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
#include "Containers/UnrealString.h"

#include "ModioValidationError.generated.h"

/**
 * @brief Wrapper struct containing information about a field validation error
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioValidationError
{
	GENERATED_BODY()
	/**
	 * @brief String description of the field that failed validation
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "mod.io|ValidationError")
	FString FieldName;

	/**
	 * @brief String description of the validation failure
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "mod.io|ValidationError")
	FString ValidationFailureDescription;
};