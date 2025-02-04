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

/**
 * @docpublic
 * @brief A utility class providing various helper functions for mod.io SDK integration in Unreal Engine.
 */
UCLASS(MinimalAPI)
class UModioSDKLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * @docpublic
	 * @brief Get the game ID specified in the mod.io project settings.
	 * @return The project game ID.
	 */
	UFUNCTION(BlueprintPure, category = "mod.io|Utilities")
	static MODIO_API FModioGameID GetProjectGameId();

	/**
	 * @docpublic
	 * @brief Get the API key specified in the mod.io project settings.
	 * @return The project API key.
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities")
	static MODIO_API FModioApiKey GetProjectApiKey();

	/**
	 * @docpublic
	 * @brief Get the environment specified in the mod.io project settings.
	 * @return The project environment type (e.g., production, staging).
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities")
	static MODIO_API EModioEnvironment GetProjectEnvironment();

	/**
	 * @docpublic
	 * @brief Get the options needed to initialize the mod.io SDK specified in the project settings.
	 * @deprecated Use GetProjectInitializeOptionsForSessionId() instead.
	 * @return The mod.io initialization options.
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities", meta = (DeprecatedFunction, DeprecationMessage = "Use GetProjectInitializeOptionsForSessionId() instead"))
	static MODIO_API FModioInitializeOptions GetProjectInitializeOptions();

	/**
	 * @docpublic
	 * @brief Get the options needed to initialize the mod.io SDK specified in the project settings.
	 * @param SessionId The session ID to initialize the project with.
	 * @return The mod.io initialization options for the provided session ID.
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities")
	static MODIO_API FModioInitializeOptions GetProjectInitializeOptionsForSessionId(const FString SessionId);

	/**
	 * @docpublic
	 * @brief Validate if the supplied email address has a valid format.
	 * @param String The email address to validate.
	 * @return True if the email address is valid.
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities")
	static MODIO_API bool IsValidEmailAddressFormat(const FString& String);

	/**
	 * @docpublic
	 * @brief Check if the supplied string has the format of a valid mod.io security code.
	 * @param String The string to check.
	 * @return True if the string matches the mod.io security code format.
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities")
	static MODIO_API bool IsValidSecurityCodeFormat(const FString& String);

	/**
	 * @docpublic
	 * @brief Get the appropriate file size unit based on the file size in bytes.
	 * @param FileSize The file size in bytes as a FModioUnsigned64.
	 * @return The desired file size unit.
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities",
			  meta = (DisplayName = "GetDesiredFileSizeUnit (FModioUnsigned64)", CompactNodeTitle = "GetFilesizeUnit"))
	static MODIO_API EFileSizeUnit GetDesiredFileSizeUnit_Unsigned64(FModioUnsigned64 FileSize);

	/**
	 * @docpublic
	 * @brief Get the appropriate file size unit based on the file size in bytes.
	 * @param FileSize The file size in bytes.
	 * @return The desired file size unit.
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities", meta = (DisplayName = "GetDesiredFileSizeUnit (int64)",
					  CompactNodeTitle = "GetFilesizeUnit", DeprecatedFunction,
					  DeprecationMessage = "The int64 version of this function is deprecated and will be removed in a future release. Please use the FModioUnsigned64 version."))
	static MODIO_API EFileSizeUnit GetDesiredFileSizeUnit(int64 FileSize);

	/**
	 * @docpublic
	 * @brief Convert a file size to a human-readable string with the appropriate unit.
	 * @param FileSize The file size in bytes as a FModioUnsigned64.
	 * @param MinDecimals Minimum number of decimal places to show.
	 * @param MaxDecimals Maximum number of decimal places to show.
	 * @param Unit The unit to use (e.g., bytes, KB, MB). If 'Largest', the unit will automatically scale to the largest
	 * possible unit.
	 * @param bIncludeUnitName Whether to include the unit name (e.g., "MB").
	 * @return A formatted string representing the file size.
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|String",
			  meta = (DisplayName = "ToString (ModioUnsigned64)", CompactNodeTitle = "Filesize"))
	static MODIO_API FText Filesize_ToString_Unsigned64(FModioUnsigned64 FileSize, int32 MinDecimals = 0, int32 MaxDecimals = 2,
											 EFileSizeUnit Unit = EFileSizeUnit::Largest, bool bIncludeUnitName = true);

	/**
	 * @docpublic
	 * @brief Convert a file size to a human-readable string with the appropriate unit.
	 * @param FileSize The file size in bytes.
	 * @param MinDecimals Minimum number of decimal places to show.
	 * @param MaxDecimals Maximum number of decimal places to show.
	 * @param Unit The unit to use (e.g., bytes, KB, MB). If 'Largest', the unit will automatically scale to the largest possible unit.
	 * @param bIncludeUnitName Whether to include the unit name (e.g., "MB").
	 * @return A formatted string representing the file size.
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|String",
			  meta = (DisplayName = "ToString (Filesize)", CompactNodeTitle = "Filesize", DeprecatedFunction,
					  DeprecationMessage = "The int64 version of this function is deprecated and will be removed in a future release. Please use the FModioUnsigned64 version."))
	static MODIO_API FText Filesize_ToString(int64 FileSize, int32 MinDecimals = 0, int32 MaxDecimals = 2,
	                                         EFileSizeUnit Unit = EFileSizeUnit::Largest, bool bIncludeUnitName = true);

	/**
	 * @docpublic
	 * @brief Convert an integer64 value to a string.
	 * @param InInt The integer to convert.
	 * @return The string representation of the integer.
	 */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToString (integer64)", CompactNodeTitle = "->", BlueprintAutocast),
		Category = "mod.io|Utilities|String")
	static MODIO_API FString Conv_Int64ToString(int64 InInt);

	/**
	 * @docpublic
	 * @brief Convert an integer64 value to text based on formatting options.
	 * @param Value The integer to format.
	 * @param bAlwaysSign Whether to always display the sign (+/-).
	 * @param bUseGrouping Whether to use grouping separators based on locale.
	 * @param MinimumIntegralDigits Minimum number of digits for the integer part.
	 * @param MaximumIntegralDigits Maximum number of digits for the integer part.
	 * @return The formatted text representation of the integer.
	 */
	UFUNCTION(BlueprintPure,
			  meta = (DisplayName = "ToText (integer64)", AdvancedDisplay = "1", BlueprintAutocast),
		Category = "mod.io|Utilities|Text")
	static MODIO_API FText Conv_Int64ToText(int64 Value, bool bAlwaysSign = false, bool bUseGrouping = true,
	                                        int32 MinimumIntegralDigits = 1, int32 MaximumIntegralDigits = 324);

	/**
	 * @docpublic
	 * @brief Calculate the percentage between two `int64` values.
	 * @param Dividend The numerator of the percentage calculation.
	 * @param Divisor The denominator of the percentage calculation.
	 * @return The floating-point result of Dividend/Divisor.
	 */
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "Get Percent (integer64/integer64)", CompactNodeTitle = "Percent",
			Keywords = "/ % percent pct"),
		Category = "mod.io|Math|integer64")
	static MODIO_API float Pct_Int64Int64(int64 Dividend, int64 Divisor);

	/**
	 * @docpublic
	 * @brief Round a number string based on the file size or speed.
	 * @param inputText The text representing the input number.
	 * @return The rounded number string.
	 */
	UFUNCTION(BlueprintPure, category = "mod.io|Utilities")
	static MODIO_API FText RoundNumberString(FText inputText);

	/**
	 * @docpublic
	 * @brief Get the time span between the present and a specified past date as a string.
	 * @param PastDateString The past date to compare against the current time.
	 * @return The time span as a string (e.g., "2 days ago").
	 */
	UFUNCTION(BlueprintPure, category = "mod.io|Utilities")
	static MODIO_API FString GetTimeSpanAsString(FString PastDateString);

	/**
	 * @docpublic
	 * @brief Shorten a large number for better readability.
	 * @param Number The large number to shorten.
	 * @return The shortened number as a string.
	 */
	UFUNCTION(BlueprintPure, category = "mod.io|Utilities")
	static MODIO_API FString GetShortenedNumberAsString(int64 Number);

	/**
	 * @docpublic
	 * @brief Get the session ID for Windows to initialize the SDK.
	 * @return The Windows session ID, or an empty string if the platform is not Windows.
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities")
	static MODIO_API FString GetDefaultSessionIdWindows();

	/**
	 * @docpublic
	 * @brief Get the language code string (ISO 639-1 format) for localization purposes.
	 * @param Language The language enum to convert.
	 * @return The language code as a string (e.g., "en", "fr", "de").
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities")
	static MODIO_API FString GetLanguageCodeString(EModioLanguage Language);

	/**
	 * @docpublic
	 * @brief Convert a language code string to the corresponding language enum.
	 * @param LanguageCode The language code string (e.g., "en", "fr", "de").
	 * @return The corresponding language enum.
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities")
	static MODIO_API EModioLanguage GetLanguageCodeFromString(FString LanguageCode);

	/**
	 * @docpublic
	 * @brief Get the purchase category ID for the store overlay UI for a given portal.
	 * @param Portal The mod.io portal.
	 * @return The purchase category ID as a string, or empty if not defined.
	 */
	UFUNCTION(BlueprintPure, category = "mod.io|Utilities")
	static MODIO_API FString GetMonetizationPurchaseCategory(EModioPortal Portal);
};
