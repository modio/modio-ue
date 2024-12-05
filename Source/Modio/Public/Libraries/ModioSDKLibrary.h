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
#include "Types/ModioAuthenticationParams.h"
#include "Types/ModioCommonTypes.h"
#include "Types/ModioInitializeOptions.h"

#include "ModioSDKLibrary.generated.h"

UCLASS(MinimalAPI)
class UModioSDKLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/**
	 * @brief Get the game id specified in the mod.io project settings
	 */
	UFUNCTION(BlueprintPure, category = "mod.io|Utilities")
	static MODIO_API FModioGameID GetProjectGameId();

	/**
	 * @brief Get the api key specified in the mod.io project settings
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities")
	static MODIO_API FModioApiKey GetProjectApiKey();

	/**
	 * @brief Get the environment specified in the mod.io project settings
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities")
	static MODIO_API EModioEnvironment GetProjectEnvironment();

	/**
	 * @brief Get the options needed to initialize the mod.io SDK specified in the project settings
	 * @deprecated Use GetProjectInitializeOptionsForSessionId() instead
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities", meta = (DeprecatedFunction, DeprecationMessage = "Use GetProjectInitializeOptionsForSessionId() instead"))
	static MODIO_API FModioInitializeOptions GetProjectInitializeOptions();

	/**
	 * @brief Get the options needed to initialize the mod.io SDK specified in the project settings
	 * @param SessionId The LocalSessionIdentifier option to initialize project with
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities")
	static MODIO_API FModioInitializeOptions GetProjectInitializeOptionsForSessionId(const FString SessionId);


	/**
	 * @brief Does a basic validation if the email address supplied has a valid form
	 * @return True if the email address has a valid format
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities")
	static MODIO_API bool IsValidEmailAddressFormat(const FString& String);

	/**
	 * @brief Checks if the string has the same format as the mod.io security code
	 * @return True if the security code has a valid format
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities")
	static MODIO_API bool IsValidSecurityCodeFormat(const FString& String);

	/**
	 * @brief Get desired file size unit based on the size of the file
	 * @return the desired file size unit
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities")
	static MODIO_API EFileSizeUnit GetDesiredFileSizeUnit(int64 FileSize);

	/**
	 * @brief Converts a filesize to a human readable string with the appropriate unit
	 *
	 * @param FileSize - Filesize in bytes
	 * @param MaxDecimals - Maximum amount of decimals to display of the filesize
	 * @param Unit - If Largest, then it tries to display the size in the largest unit that will have a integral
	 * part > 0, else it displays the filesize in the specified unit
	 * @return A text formatted from your specifications
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|String",
			  meta = (DisplayName = "ToString (Filesize)", CompactNodeTitle = "Filesize"))
	static MODIO_API FText Filesize_ToString(int64 FileSize, int32 MinDecimals = 0, int32 MaxDecimals = 2,
											 EFileSizeUnit Unit = EFileSizeUnit::Largest, bool bIncludeUnitName = true);

	/** Converts an integer64 value to a string */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToString (integer64)", CompactNodeTitle = "->", BlueprintAutocast),
			  Category = "mod.io|Utilities|String")
	static MODIO_API FString Conv_Int64ToString(int64 InInt);

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
	static MODIO_API FText Conv_Int64ToText(int64 Value, bool bAlwaysSign = false, bool bUseGrouping = true,
											int32 MinimumIntegralDigits = 1, int32 MaximumIntegralDigits = 324);
	/**
	 * @brief Calculates percentage using two `int64` params
	 * @return The floating point result of Dividend/Divisor with no checks
	*/
	UFUNCTION(BlueprintPure,
			  meta = (DisplayName = "Get Percent (integer64/integer64)", CompactNodeTitle = "Percent",
					  Keywords = "/ % percent pct"),
			  Category = "mod.io|Math|integer64")
	static MODIO_API float Pct_Int64Int64(int64 Dividend, int64 Divisor);


	/**
	 * @brief Sets the correct decimals depending on the file size or speed
	 *
	 * @param Input value of bytes per second as FText
	 */
	UFUNCTION(BlueprintPure, category = "mod.io|Utilities")
	static MODIO_API FText RoundNumberString(FText inputText);

	/**
	 * @brief Gets the time span between present and specified past date FString
	 *
	 * @param Input value of date FString in the past
	 */
	UFUNCTION(BlueprintPure, category = "mod.io|Utilities")
	static MODIO_API FString GetTimeSpanAsString(FString PastDateString);

	/**
	 * @brief Shortens the specified large number
	 *
	 * @param Input value of large number to shorten
	 */
	UFUNCTION(BlueprintPure, category = "mod.io|Utilities")
	static MODIO_API FString GetShortenedNumberAsString(int64 Number);

	/**
	* @brief Get Session Id for Windows for initialization of the SDK
	* @return The windows session id, or an empty string if you are not on Windows
	*/
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities")
	static MODIO_API FString GetDefaultSessionIdWindows();

	/**
	 * @brief Get language code string. This can be used for localization purposes
	 * @param Language The language code to convert to string
	 * @return The language code as a string (e.g. "en", "fr", "de")
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities")
	static MODIO_API FString GetLanguageCodeString(EModioLanguage Language);

	/**
	 * @brief Get language code enum from string in ISO 639-1 format. This can be used for localization purposes
	 * @param LanguageCode The language code as a string (e.g. "en", "fr", "de")
	 * @return The language code as an enum
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities")
	static MODIO_API EModioLanguage GetLanguageCodeFromString(FString LanguageCode);

	/**
	 * @brief Get the purchase category to pass to the store overlay UI for a given portal.
	 * @return string of ID for the purchase category for a given Portal in use, returns empty if the given key was not found/not defined
	 */
	UFUNCTION(BlueprintPure, category = "mod.io|Utilities")
	static MODIO_API FString GetMonetizationPurchaseCategory(EModioPortal Portal);
};
