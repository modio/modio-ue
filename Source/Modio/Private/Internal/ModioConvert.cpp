#include "Internal/ModioConvert.h"

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

std::chrono::system_clock::time_point ToModioDateTime(FDateTime Time)
{
	return std::chrono::system_clock::time_point(std::chrono::system_clock::duration(Time.ToUnixTimestamp()));
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



FString ToUnreal(const Modio::filesystem::path& Path)
{
	return UTF8_TO_TCHAR(Path.generic_u8string().c_str());
}

FDateTime ToUnrealDateTime(std::int64_t UnixTimestamp)
{
	return FDateTime::FromUnixTimestamp(UnixTimestamp);
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
	}

	checkf(false, TEXT("Missed a case in ToModio(EModioLanguage Language)"));
	return Modio::Language::English;
}

FModioModID ToUnreal(Modio::ModID Value)
{
	return FModioModID(Value);
}

FModioFileMetadataID ToUnreal(Modio::FileMetadataID Value)
{
	return FModioFileMetadataID(Value);
}
