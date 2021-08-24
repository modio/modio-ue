#pragma once
#include "Internal/ModioConvert.h"
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
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	FString FieldName;

	/**
	 * @brief String description of the validation failure
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	FString ValidationFailureDescription;
};