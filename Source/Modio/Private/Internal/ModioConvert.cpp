/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "Internal/ModioConvert.h"

#include "Convert/Optional.h"
#include "Types/ModioUnsigned64.h"

std::string ToModio(const FString& String)
{
	return std::string(TCHAR_TO_UTF8(*String));
}

std::vector<std::string> ToModio(const TArray<FString>& StringArray)
{
	std::vector<std::string> Result;
	Result.reserve(StringArray.Num());
	for (const FString& It : StringArray)
	{
		Result.emplace_back(TCHAR_TO_UTF8(*It));
	}
	return Result;
}

std::vector<Modio::ModID> ToModio(const TArray<FModioModID>& ModIDArray)
{
	std::vector<Modio::ModID> Result;
	Result.reserve(ModIDArray.Num());
	for (const FModioModID& It : ModIDArray)
	{
		Result.emplace_back(ToModio(It));
	}
	return Result;
}

std::map<std::string, std::string> ToModio(const TMap<FString, FString>& StringMap)
{
	std::map<std::string, std::string> Result;
	for (const TPair<FString, FString>& It : StringMap)
	{
		Result.emplace(std::make_pair(ToModio(It.Key), ToModio(It.Value)));
	}
	return Result;
}

std::chrono::system_clock::time_point ToModioDateTime(FDateTime Time)
{
	std::chrono::seconds DurationSinceEpoch(Time.ToUnixTimestamp());
	return std::chrono::system_clock::time_point(DurationSinceEpoch);
}

int64 ToUnreal(std::int64_t Value)
{
	return Value;
}

bool ToUnreal(bool Value)
{
	return Value;
}

double ToUnreal(double Value)
{
	return Value;
}

uint64 ToUnreal(std::size_t Value)
{
	static_assert(sizeof(std::size_t) == sizeof(uint64), "size_t is not 64-bits wide. Are you in x64 configuration?");
	return Value;
}

FString ToUnreal(const std::string& String)
{
	return UTF8_TO_TCHAR(String.c_str());
}

FModioUnsigned64 ToUnreal(const Modio::FileSize& In)
{
	return FModioUnsigned64(In);
}

FDateTime ToUnrealDateTime(std::int64_t UnixTimestamp)
{
	return FDateTime::FromUnixTimestamp(UnixTimestamp);
}

FText ToUnrealText(const std::string& String)
{
	return FText::FromString(UTF8_TO_TCHAR(String.c_str()));
}

FString ToUnrealString(EModioLanguage Language)
{
	return FString(ToUnreal(Modio::Detail::ToString(ToModio(Language))));
}

Modio::ApiKey ToModio(const FModioApiKey& In)
{
	return Modio::ApiKey(TCHAR_TO_UTF8(*In.ToString()));
}

Modio::EmailAddress ToModio(const FModioEmailAddress& In)
{
	return Modio::EmailAddress(TCHAR_TO_UTF8(*In.ToString()));
}

Modio::EmailAuthCode ToModio(const FModioEmailAuthCode& In)
{
	return Modio::EmailAuthCode(TCHAR_TO_UTF8(*In.ToString()));
}

Modio::ModID ToModio(const FModioModID& In)
{
	return Modio::ModID(In.ModID);
}

Modio::GameID ToModio(const FModioGameID& In)
{
	return Modio::GameID(In.GameId);
}
Modio::FileMetadataID ToModio(const FModioFileMetadataID& In)
{
	return Modio::FileMetadataID(In.FileMetadataID);
}
Modio::UserID ToModio(const FModioUserID& In)
{
	return Modio::UserID(In.UserID);
}

Modio::LogLevel ToModio(EModioLogLevel UnrealLogLevel)
{
	switch (UnrealLogLevel)
	{
		case EModioLogLevel::Error:
			return Modio::LogLevel::Error;
		case EModioLogLevel::Info:
			return Modio::LogLevel::Info;
		case EModioLogLevel::Trace:
			return Modio::LogLevel::Trace;
		case EModioLogLevel::Warning:
			return Modio::LogLevel::Warning;
		case EModioLogLevel::Detailed:
			return Modio::LogLevel::Detailed;
	}

	checkf(false, TEXT("Missed a case in ToModio(EModioLogLevel UnrealLogLevel)"));
	return Modio::LogLevel::Info;
}

Modio::AvatarSize ToModio(EModioAvatarSize AvatarSize)
{
	switch (AvatarSize)
	{
		case EModioAvatarSize::Original:
			return Modio::AvatarSize::Original;
		case EModioAvatarSize::Thumb50:
			return Modio::AvatarSize::Thumb50;
		case EModioAvatarSize::Thumb100:
			return Modio::AvatarSize::Thumb100;
	}

	checkf(false, TEXT("Missed a case in ToModio(EModioAvatarSize AvatarSize)"));
	return Modio::AvatarSize::Thumb50;
}

Modio::GallerySize ToModio(EModioGallerySize GallerySize)
{
	switch (GallerySize)
	{
		case EModioGallerySize::Original:
			return Modio::GallerySize::Original;
		case EModioGallerySize::Thumb320:
			return Modio::GallerySize::Thumb320;
		case EModioGallerySize::Thumb1280:
			return Modio::GallerySize::Thumb1280;
	}

	checkf(false, TEXT("Missed a case in ToModio(EModioGallerySize GallerySize)"));
	return Modio::GallerySize::Thumb320;
}

Modio::LogoSize ToModio(EModioLogoSize LogoSize)
{
	switch (LogoSize)
	{
		case EModioLogoSize::Original:
			return Modio::LogoSize::Original;
		case EModioLogoSize::Thumb320:
			return Modio::LogoSize::Thumb320;
		case EModioLogoSize::Thumb640:
			return Modio::LogoSize::Thumb640;
		case EModioLogoSize::Thumb1280:
			return Modio::LogoSize::Thumb1280;
	}

	checkf(false, TEXT("Missed a case in ToModio(EModioLogoSize LogoSize)"));
	return Modio::LogoSize::Thumb320;
}

Modio::Environment ToModio(EModioEnvironment Environment)
{
	switch (Environment)
	{
		case EModioEnvironment::Test:
			return Modio::Environment::Test;
		case EModioEnvironment::Live:
			return Modio::Environment::Live;
	}

	checkf(false, TEXT("Missed a case in ToModio(EModioEnvironment Environment)"));
	return Modio::Environment::Test;
}

Modio::ModfilePlatform ToModio(EModioModfilePlatform Platform)
{
	switch (Platform)
	{
		case EModioModfilePlatform::Android:
			return Modio::ModfilePlatform::Android;
		case EModioModfilePlatform::iOS:
			return Modio::ModfilePlatform::iOS;
		case EModioModfilePlatform::Linux:
			return Modio::ModfilePlatform::Linux;
		case EModioModfilePlatform::Mac:
			return Modio::ModfilePlatform::Mac;
		case EModioModfilePlatform::Oculus:
			return Modio::ModfilePlatform::Oculus;
		case EModioModfilePlatform::PS4:
			return Modio::ModfilePlatform::PS4;
		case EModioModfilePlatform::PS5:
			return Modio::ModfilePlatform::PS5;
		case EModioModfilePlatform::Switch:
			return Modio::ModfilePlatform::Switch;
		case EModioModfilePlatform::Windows:
			return Modio::ModfilePlatform::Windows;
		case EModioModfilePlatform::XboxOne:
			return Modio::ModfilePlatform::XboxOne;
		case EModioModfilePlatform::XboxSeriesX:
			return Modio::ModfilePlatform::XboxSeriesX;
		case EModioModfilePlatform::Source:
			return Modio::ModfilePlatform::Source;
	}
	checkf(false, TEXT("Unhandled value in ToModio(EModioModfilePlatform Platform)"));
	return Modio::ModfilePlatform::Windows;
}

Modio::Portal ToModio(EModioPortal Portal)
{
	switch (Portal)
	{
		case EModioPortal::None:
			return Modio::Portal::None;
			break;
		case EModioPortal::Apple:
			return Modio::Portal::Apple;
			break;
		case EModioPortal::EpicGamesStore:
			return Modio::Portal::EpicGamesStore;
			break;
		case EModioPortal::GOG:
			return Modio::Portal::GOG;
			break;
		case EModioPortal::Google:
			return Modio::Portal::Google;
			break;
		case EModioPortal::Itchio:
			return Modio::Portal::Itchio;
			break;
		case EModioPortal::Nintendo:
			return Modio::Portal::Nintendo;
			break;
		case EModioPortal::PSN:
			return Modio::Portal::PSN;
			break;
		case EModioPortal::Steam:
			return Modio::Portal::Steam;
			break;
		case EModioPortal::XboxLive:
			return Modio::Portal::XboxLive;
			break;
	}
	checkf(false, TEXT("Missed a case in ToModio(EModioPortal)"));
	return Modio::Portal::None;
}

Modio::Language ToModio(EModioLanguage Language)
{
	switch (Language)
	{
		case EModioLanguage::English:
			return Modio::Language::English;
		case EModioLanguage::Bulgarian:
			return Modio::Language::Bulgarian;
		case EModioLanguage::French:
			return Modio::Language::French;
		case EModioLanguage::German:
			return Modio::Language::German;
		case EModioLanguage::Italian:
			return Modio::Language::Italian;
		case EModioLanguage::Polish:
			return Modio::Language::Polish;
		case EModioLanguage::Portuguese:
			return Modio::Language::Portuguese;
		case EModioLanguage::Hungarian:
			return Modio::Language::Hungarian;
		case EModioLanguage::Japanese:
			return Modio::Language::Japanese;
		case EModioLanguage::Korean:
			return Modio::Language::Korean;
		case EModioLanguage::Russian:
			return Modio::Language::Russian;
		case EModioLanguage::Spanish:
			return Modio::Language::Spanish;
		case EModioLanguage::Thai:
			return Modio::Language::Thai;
		case EModioLanguage::ChineseSimplified:
			return Modio::Language::ChineseSimplified;
		case EModioLanguage::ChineseTraditional:
			return Modio::Language::ChineseTraditional;
		case EModioLanguage::Turkish:
			return Modio::Language::Turkish;
		case EModioLanguage::Ukrainian:
			return Modio::Language::Ukrainian;
		case EModioLanguage::Arabic:
			return Modio::Language::Arabic;
	}

	checkf(false, TEXT("Missed a case in ToModio(EModioLanguage Language)"));
	return Modio::Language::English;
}

Modio::EntitlementParams ToModio(const FModioEntitlementParams& InParams)
{
	Modio::EntitlementParams Params;
	Params.ExtendedParameters = ToModio(InParams.ExtendedParameters);
	return Params;
}

Modio::MetricsSessionParams ToModio(const FModioMetricsSessionParams& InParams)
{
	Modio::MetricsSessionParams Params;
	Params.SessionId = ToModioOptional<Modio::Guid>(InParams.SessionId);
	Params.ModIds = ToModio(InParams.ModIds);
	return Params;
}

Modio::Guid ToModio(const FModioGuid& In)
{
	if (In.IsValid())
	{
		return Modio::Guid(TCHAR_TO_UTF8(*In.ToString()));
	}

	return Modio::Guid::InvalidGuid();
}

FModioModID ToUnreal(Modio::ModID Value)
{
	return FModioModID(Value);
}

FModioGameID ToUnreal(Modio::GameID Value)
{
	return FModioGameID(Value);
}

FModioFileMetadataID ToUnreal(Modio::FileMetadataID Value)
{
	return FModioFileMetadataID(Value);
}

EModioModfilePlatform ToUnreal(const Modio::ModfilePlatform& PlatformIn)
{
	switch (PlatformIn)
	{
		case Modio::ModfilePlatform::Windows:
			return EModioModfilePlatform::Windows;
		case Modio::ModfilePlatform::Mac:
			return EModioModfilePlatform::Mac;
		case Modio::ModfilePlatform::Linux:
			return EModioModfilePlatform::Linux;
		case Modio::ModfilePlatform::Android:
			return EModioModfilePlatform::Android;
		case Modio::ModfilePlatform::iOS:
			return EModioModfilePlatform::iOS;
		case Modio::ModfilePlatform::XboxOne:
			return EModioModfilePlatform::XboxOne;
		case Modio::ModfilePlatform::XboxSeriesX:
			return EModioModfilePlatform::XboxSeriesX;
		case Modio::ModfilePlatform::PS4:
			return EModioModfilePlatform::PS4;
		case Modio::ModfilePlatform::PS5:
			return EModioModfilePlatform::PS5;
		case Modio::ModfilePlatform::Switch:
			return EModioModfilePlatform::Switch;
		case Modio::ModfilePlatform::Oculus:
			return EModioModfilePlatform::Oculus;
		case Modio::ModfilePlatform::Source:
			return EModioModfilePlatform::Source;
	}
	checkf(false, TEXT("Missed a case in ToUnreal(const Modio::ModfilePlatform& PlatformIn)"));
	return EModioModfilePlatform::Windows;
}

EModioModChangeType ToUnreal(const Modio::UserSubscriptionList::ChangeType& In)
{
	switch (In)
	{
		case Modio::UserSubscriptionList::ChangeType::Added:
			return EModioModChangeType::Added;
		case Modio::UserSubscriptionList::ChangeType::Removed:
			return EModioModChangeType::Removed;
		case Modio::UserSubscriptionList::ChangeType::Updated:
			return EModioModChangeType::Updated;
		default:
			return EModioModChangeType::Removed;
	}
}

EModioLanguage ToUnreal(const Modio::Language& In)
{
	switch (In)
	{
		case Modio::Language::English:
			return EModioLanguage::English;
		case Modio::Language::Bulgarian:
			return EModioLanguage::Bulgarian;
		case Modio::Language::French:
			return EModioLanguage::French;
		case Modio::Language::German:
			return EModioLanguage::German;
		case Modio::Language::Italian:
			return EModioLanguage::Italian;
		case Modio::Language::Polish:
			return EModioLanguage::Polish;
		case Modio::Language::Portuguese:
			return EModioLanguage::Portuguese;
		case Modio::Language::Hungarian:
			return EModioLanguage::Hungarian;
		case Modio::Language::Japanese:
			return EModioLanguage::Japanese;
		case Modio::Language::Korean:
			return EModioLanguage::Korean;
		case Modio::Language::Russian:
			return EModioLanguage::Russian;
		case Modio::Language::Spanish:
			return EModioLanguage::Spanish;
		case Modio::Language::Thai:
			return EModioLanguage::Thai;
		case Modio::Language::ChineseSimplified:
			return EModioLanguage::ChineseSimplified;
		case Modio::Language::ChineseTraditional:
			return EModioLanguage::ChineseTraditional;
	}

	checkf(false, TEXT("Missed a case in ToUnreal(Modio::Language Language)"));
	return EModioLanguage::English;
}
