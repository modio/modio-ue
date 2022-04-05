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
#include "Types/ModioInitializeOptions.h"
#include "Types/ModioCommonTypes.h"

#include "ModioSDKLibrary.generated.h"


UENUM(BlueprintType)
enum EFileSizeUnit
{
	Largest = 0, // Will take the largest one that becomes a number larger than 1 (i.e, 1300mb becomes 1.23MB)
	B = 1,
	KB = 1024,
	MB = 1024 * 1024,
	GB = 1024 * 1024 * 1024
};

UCLASS(MinimalAPI)
class UModioSDKLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/**
	 * Get the game id specified in the mod.io project settings
	 */
	UFUNCTION(BlueprintPure, category = "mod.io|Utilities")
	static MODIO_API FModioGameID GetProjectGameId();

	/**
	 * Get hte api key specified in the mod.io project settings
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities")
	static MODIO_API FModioApiKey GetProjectApiKey();

	/**
	 * Get the environment specified in the mod.io project settings
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities")
	static MODIO_API EModioEnvironment GetProjectEnvironment();

	/**
	 * Get the options needed to initialize the mod.io SDK specified in the project settings
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities")
	static MODIO_API FModioInitializeOptions GetProjectInitializeOptions();
	
#if WITH_DEV_AUTOMATION_TESTS
	/// @brief Gets the automation test options. not exposed to BP to prevent accidental use by developers in BP
	static MODIO_API FModioInitializeOptions GetAutomationTestOptions();
#endif

	/**
	 * Does a basic validation if the email address supplied has a valid form
	 * @return true if the email address has a valid format
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities")
	static MODIO_API bool IsValidEmailAddressFormat(const FString& String);

	/**
	 * Checks if the string has the same format as the mod.io security code
	 * @return true if the security code has a valid format
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities")
	static MODIO_API bool IsValidSecurityCodeFormat(const FString& String);

	/**
	 * Converts a filesize to a human readable string with the appropriate unit
	 *
	 * @param FileSize - Filesize in bytes
	 * @param MaxDecimals - Maximum amount of decimals to display of the filesize
	 * @param Unit - If Largest, then it tries to display the size in the largest unit that will have a integral
	 * part > 0, else it displays the filesize in the specified unit
	 * @return A text formatted from your specifications
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|String",
			  meta = (DisplayName = "ToString (Filesize)", CompactNodeTitle = "Filesize"))
	static FText Filesize_ToString(int64 FileSize, int32 MaxDecimals = 2, EFileSizeUnit Unit = EFileSizeUnit::Largest);

	/** Converts an integer64 value to a string */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToString (integer64)", CompactNodeTitle = "->", BlueprintAutocast),
			  Category = "mod.io|Utilities|String")
	static FString Conv_Int64ToString(int64 InInt);

	/**
	 * @brief Converts a passed in integer to text based on formatting options
	 *
	 * @param Value the integer to format
	 * @param bAlwaysSign if true, the sign (+/-) is always present
	 * @param bUseGrouping If true, then the text is divided up in groups with separators based on computer locale
	 * @param MinimumIntegralDigits If the amount of numbers is less than this, then pad out with 0 at start of string
	 * @param MaximumIntegralDigits
	 */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToText (integer64)", AdvancedDisplay = "1", BlueprintAutocast),
			  Category = "mod.io|Utilities|Text")
	static FText Conv_Int64ToText(int64 Value, bool bAlwaysSign = false, bool bUseGrouping = true,
								  int32 MinimumIntegralDigits = 1, int32 MaximumIntegralDigits = 324);

	/** @brief Dividend/Divisor and return the floating point result with no checks **/
	UFUNCTION(BlueprintPure,
			  meta = (DisplayName = "Get Percent (integer64/integer64)", CompactNodeTitle = "Percent",
					  Keywords = "/ % percent pct"),
			  Category = "mod.io|Math|integer64")
	static float Pct_Int64Int64(int64 Dividend, int64 Divisor);
};
