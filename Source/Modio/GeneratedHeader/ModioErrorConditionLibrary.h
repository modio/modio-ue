#pragma once
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Types/ModioErrorCode.h"

#include "ModioErrorConditionLibrary.generated.h"

UENUM(BlueprintType)
enum class EModioErrorCondition: uint8
{
	NoError = 0,
	NetworkError = (uint8) Modio::ErrorConditionTypes::NetworkError UMETA(ToolTip="When this condition is true, the error code represents a connection or HTTP error between the client and the mod.io server.") ,
	ConfigurationError = (uint8) Modio::ErrorConditionTypes::ConfigurationError UMETA(ToolTip="When this condition is true, the error code indicates the SDK's configuration is not valid - the game ID or API key are incorrect or the game has been deleted.") ,
	InvalidArgsError = (uint8) Modio::ErrorConditionTypes::InvalidArgsError UMETA(ToolTip="When this condition is true, the error code indicates the arguments passed to the function have failed validation or were otherwise invalid.") ,
	FilesystemError = (uint8) Modio::ErrorConditionTypes::FilesystemError UMETA(ToolTip="When this condition is true, the error code indicates a permission or IO error when accessing local filesystem data.") ,
	InternalError = (uint8) Modio::ErrorConditionTypes::InternalError UMETA(ToolTip="When this condition is true, the error code represents an internal SDK error - please inform mod.io of the error code value.") ,
	EntityNotFoundError = (uint8) Modio::ErrorConditionTypes::EntityNotFoundError UMETA(ToolTip="When this condition is true, the error code indicates that a specified game, mod, media file or mod file was not found.") ,
	UserTermsOfUseError = (uint8) Modio::ErrorConditionTypes::UserTermsOfUseError UMETA(ToolTip="When this condition is true, the error code indicates that the user has not yet accepted the mod.io Terms of Use.") ,
};

UCLASS()
class UModioErrorConditionLibrary : public UBlueprintFunctionLibrary
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
	static bool ErrorCodeMatches(FModioErrorCode ErrorCode, EModioErrorCondition Condition)
	{
		if (Condition == EModioErrorCondition::NoError)
		{
			return (bool)ErrorCode.GetRawErrorCode();
		}
		return Modio::ErrorCodeMatches(ErrorCode.GetRawErrorCode(), static_cast<Modio::ErrorConditionTypes>(Condition));
	}

};