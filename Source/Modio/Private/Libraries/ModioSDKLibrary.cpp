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
#include "Internal/Convert/InitializeOptions.h"
#include "Modio.h"
#include "ModioSettings.h"
#include "Internal/ModioConvert.h"


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
	Options.PortalInUse = Settings->DefaultPortal;
	Options.bUseBackgroundThread = Settings->bUseBackgroundThread;

	return Options;
}

FModioInitializeOptions UModioSDKLibrary::GetProjectInitializeOptionsForSessionId(const FString SessionId)
{
	const UModioSettings* Settings = GetDefault<UModioSettings>();

	FModioInitializeOptions Options;
	Options.ApiKey = FModioApiKey(Settings->ApiKey);
	Options.GameEnvironment = Settings->Environment;
	Options.GameId = FModioGameID(Settings->GameId);
	Options.PortalInUse = Settings->DefaultPortal;
	Options.LocalSessionIdentifier = SessionId;
	Options.bUseBackgroundThread = Settings->bUseBackgroundThread;

	if (SessionId.IsEmpty())
	{
		UE_LOG(LogModio, Error, TEXT("SessionID cannot be empty for GetProjectInitializeOptions"));
	}
	return Options;
}


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
}

EFileSizeUnit UModioSDKLibrary::GetDesiredFileSizeUnit(int64 FileSize)
{
	if (FileSize > GB)
	{
		return EFileSizeUnit::GB;
	}
	if (FileSize > MB)
	{
		return EFileSizeUnit::MB;
	}
	if (FileSize > KB)
	{
		return EFileSizeUnit::KB;
	}
	return EFileSizeUnit::B;
}

FText UModioSDKLibrary::Filesize_ToString(int64 FileSize, int32 MinDecimals/* = 0*/, int32 MaxDecimals /** = 2*/,
										  EFileSizeUnit Unit /**= EFileSizeUnit::Largest*/, bool bIncludeUnitName /**= true*/)
{
	static const int32 KB = 1024;
	static const int32 MB = 1024 * 1024;
	static const int32 GB = 1024 * 1024 * 1024;

	if (Unit == EFileSizeUnit::Largest)
	{
		Unit = GetDesiredFileSizeUnit(FileSize);
	}

	const double InNewUnit = FileSize / static_cast<double>(Unit);

	FFormatNamedArguments Args;

	FNumberFormattingOptions FormatRules;
	FormatRules.MinimumFractionalDigits = MinDecimals;
	FormatRules.MaximumFractionalDigits = MaxDecimals;
	FormatRules.MinimumIntegralDigits = 1;

	Args.Add("Number", FText::AsNumber(InNewUnit, &FormatRules));
	Args.Add("Unit", bIncludeUnitName ? FText::FromString(ToString(Unit)) : FText::GetEmpty());
	FText FormatString = FText::FromString(TEXT("{Number}{Unit}"));
	FText::FindText(FTextKey("Internationalization"), FTextKey("ComputerMemoryFormatting"), FormatString) ;
	return FText::Format(FormatString, Args);
}

bool UModioSDKLibrary::IsValidEmailAddressFormat(const FString& String)
{
	if (String.IsEmpty())
	{
		return false;
	}

	const FRegexPattern Pattern(TEXT("^\\w+([-+.']\\w+)*@\\w+([-.]\\w+)*\\.\\w+([-.]\\w+)*$"));
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

FText UModioSDKLibrary::RoundNumberString(FText inputText)
{
	FString BytesPerSecondFromFText = inputText.ToString();

	TArray<FString> Parsed;
	const TCHAR* Delims[] = {TEXT("."), TEXT(","), TEXT(" ")};
	BytesPerSecondFromFText.ParseIntoArray(Parsed, Delims, 3);

	FText FinalText;
	bool bNeedsDecimals = true;
	FString decimals = "0";
	FString num = "0";
	FString units = "";

	for (int i = Parsed.Num(); i >= 0; i--)
	{
		// Units:
		if (i == Parsed.Num() - 1 && (Parsed[i].Contains("K") || Parsed[i].Contains("M")))
		{
			bNeedsDecimals = false;
			units = " " + Parsed[i];
		}

		// Decimals:
		if (i == (Parsed.Num() - 2) && i != 0) decimals = Parsed[i].Mid(0, 1);

		// Numbers:
		if (i == 0) num = Parsed[i];
	}

	FString FinalString = "";
	bNeedsDecimals ? FinalString.Append(num).Append(",").Append(decimals).Append(units) : FinalString.Append(num).Append(units);
	FinalText = FText::FromString(FinalString);

	return FinalText;
}

FString UModioSDKLibrary::GetTimeSpanAsString(FString PastDateString)
{
	FDateTime Present = FDateTime::Now();
	FDateTime Past;
	if (!FDateTime::Parse(PastDateString, Past)) 
	{
		return PastDateString;
	}
	FTimespan TimeSpan = Present - Past;

	int32 Days = TimeSpan.GetDays();
	int32 Months = TimeSpan.GetDays() / 30;
	int32 Years = TimeSpan.GetDays() / 365;

	if (Years > 0)
	{
		return FString::Printf(TEXT("%d year%s ago"), Years, *(Years == 1 ? FString("") : FString("s")));
	}
	else if (Months > 0)
	{
		return FString::Printf(TEXT("%d month%s ago"), Months, *(Months == 1 ? FString("") : FString("s")));
	}
	else if (Days > 0)
	{
		return FString::Printf(TEXT("%d day%s ago"), Days, *(Days == 1 ? FString("") : FString("s")));
	}

	return FString("Today");
}

FString UModioSDKLibrary::GetShortenedNumberAsString(int64 Number)
{
	int64 Thousands = Number / 1000;
	int64 Millions = Number / 1000000;
	int64 Billions = Number / 1000000000;
	int64 Trillions = Number / 1000000000000;

	if (Trillions > 0)
	{
		return FString::Printf(TEXT("%lld%s"), Trillions, *FString("T"));
	}
	else if (Billions > 0)
	{
		return FString::Printf(TEXT("%lld%s"), Billions, *FString("B"));
	}
	else if (Millions > 0)
	{
		return FString::Printf(TEXT("%lld%s"), Millions, *FString("M"));
	}
	else if (Thousands > 0)
	{
		return FString::Printf(TEXT("%lld%s"), Thousands, *FString("K"));
	}

	return FString::FromInt(Number);
}

FString UModioSDKLibrary::GetDefaultSessionIdWindows()
{
#if PLATFORM_WINDOWS
	return FString(GetUserSidString().c_str());
#else
	return FString(TEXT(""));
#endif
}

FString UModioSDKLibrary::GetLanguageCodeString(EModioLanguage Language)
{
	return ToUnrealString(Language);
}

EModioLanguage UModioSDKLibrary::GetLanguageCodeFromString(FString LanguageCode)
{
	for (EModioLanguage Language : TEnumRange<EModioLanguage>())
	{
		if (ToUnrealString(Language) == LanguageCode)
		{
			return Language;
		}
	}
	return EModioLanguage::English;
}
