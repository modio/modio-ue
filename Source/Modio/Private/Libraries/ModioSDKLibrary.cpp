/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "Libraries/ModioSDKLibrary.h"
#include "Internationalization/Regex.h"
#include "ModioSettings.h"

FModioGameID UModioSDKLibrary::GetProjectGameId()
{
	return FModioGameID(GetDefault<UModioSettings>()->GameId);
}

FModioApiKey UModioSDKLibrary::GetProjectApiKey()
{
	return FModioApiKey(GetDefault<UModioSettings>()->ApiKey);
}

EModioEnvironment UModioSDKLibrary::GetProjectEnvironment()
{
	return GetDefault<UModioSettings>()->Environment;
}

FModioInitializeOptions UModioSDKLibrary::GetProjectInitializeOptions()
{
	const UModioSettings* Settings = GetDefault<UModioSettings>();

	FModioInitializeOptions Options;
	Options.ApiKey = FModioApiKey(Settings->ApiKey);
	Options.GameEnvironment = Settings->Environment;
	Options.GameId = FModioGameID(Settings->GameId);

	return Options;
}

#if WITH_DEV_AUTOMATION_TESTS
FModioInitializeOptions UModioSDKLibrary::GetAutomationTestOptions()
{
	#if WITH_EDITORONLY_DATA
	const UModioSettings* Settings = GetDefault<UModioSettings>();
	FModioInitializeOptions Options = Settings->AutomationTestOptions;
	if (!Settings->AutomationSessionID.IsEmpty())
	{
		Options.LocalSessionIdentifier = Settings->AutomationSessionID;
	}
	return Options;
	#else
	return FModioInitializeOptions {};
	#endif
}
#endif

static FString ToString(EFileSizeUnit Unit)
{
	switch (Unit)
	{
		case EFileSizeUnit::B:
			return TEXT("bytes");
		case EFileSizeUnit::KB:
			return TEXT("KB");
		case EFileSizeUnit::MB:
			return TEXT("MB");
		case EFileSizeUnit::GB:
			return TEXT("GB");
		default:
			return TEXT("Unknown unit");
	}

	return TEXT("Unknown unit");
}

FText UModioSDKLibrary::Filesize_ToString(int64 FileSize, int32 MaxDecimals /** = 2*/,
										  EFileSizeUnit Unit /**= EFileSizeUnit::Largest*/)
{
	static const int32 KB = 1024;
	static const int32 MB = 1024 * 1024;
	static const int32 GB = 1024 * 1024 * 1024;

	if (Unit == EFileSizeUnit::Largest)
	{
		if (FileSize > GB)
		{
			Unit = EFileSizeUnit::GB;
		}
		else if (FileSize > MB)
		{
			Unit = EFileSizeUnit::MB;
		}
		else if (FileSize > KB)
		{
			Unit = EFileSizeUnit::KB;
		}
		else
		{
			Unit = EFileSizeUnit::B;
		}
	}

	const double InNewUnit = FileSize / static_cast<double>(Unit);

	FFormatNamedArguments Args;

	FNumberFormattingOptions FormatRules;
	FormatRules.MinimumFractionalDigits = 0;
	FormatRules.MaximumFractionalDigits = MaxDecimals;
	FormatRules.MinimumIntegralDigits = 1;

	Args.Add(TEXT("FileSize"), FText::AsNumber(InNewUnit, &FormatRules));
	Args.Add(TEXT("UnitName"), FText::FromString(ToString(Unit)));

	return FText::Format(FTextFormat::FromString(TEXT("{FileSize}{UnitName}")), Args);
}

bool UModioSDKLibrary::IsValidEmailAddressFormat(const FString& String)
{
	// Regex for validating email adress found here: https://mylittledevblog.com/2018/02/15/ue4-email-validation/
	// I would prefer to use https://stackoverflow.com/a/201378/12018052, but that doesn't work
	const FRegexPattern Pattern(TEXT("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+"));
	FRegexMatcher Matcher(Pattern, String);

	return Matcher.FindNext();
}

bool UModioSDKLibrary::IsValidSecurityCodeFormat(const FString& String)
{
	// Make a copy so that we can do inline operations on the string more efficiently
	FString StringCopy = String;
	StringCopy.TrimStartAndEndInline();
	return StringCopy.Len() == 5;
}

FString UModioSDKLibrary::Conv_Int64ToString(int64 InInt)
{
	return FString::Printf(TEXT("%lld"), InInt);
}

FText UModioSDKLibrary::Conv_Int64ToText(int64 Value, bool bAlwaysSign /* = false*/, bool bUseGrouping /* = true*/,
										 int32 MinimumIntegralDigits /* = 1*/, int32 MaximumIntegralDigits /* = 324*/)
{
	// Only update the values that need to be changed from the default FNumberFormattingOptions,
	// as this lets us use the default formatter if possible (which is a performance win!)
	FNumberFormattingOptions NumberFormatOptions;
	NumberFormatOptions.AlwaysSign = bAlwaysSign;
	NumberFormatOptions.UseGrouping = bUseGrouping;
	NumberFormatOptions.MinimumIntegralDigits = MinimumIntegralDigits;
	NumberFormatOptions.MaximumIntegralDigits = MaximumIntegralDigits;

	return FText::AsNumber(Value, &NumberFormatOptions);
}

float UModioSDKLibrary::Pct_Int64Int64(int64 Part, int64 Whole)
{
	return Part / static_cast<double>(Whole);
}
