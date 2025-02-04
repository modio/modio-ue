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
#include "Containers/UnrealString.h"

#include "ModioValidationError.generated.h"

/**
 * @docpublic
 * @brief Wrapper struct containing information about a field validation error
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioValidationError
{
	GENERATED_BODY()

	/** 
	 * @docpublic
	 * @brief String description of the field that failed validation
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "mod.io|ValidationError")
	FString FieldName;

	/** 
	 * @docpublic
	 * @brief String description of the validation failure
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "mod.io|ValidationError")
	FString ValidationFailureDescription;
};
