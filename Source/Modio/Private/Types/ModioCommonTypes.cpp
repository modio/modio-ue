#include "Types/ModioCommonTypes.h"

FModioModID::FModioModID() : ModID(INDEX_NONE) {}

FModioGameID::FModioGameID() : GameID(INDEX_NONE) {}

FModioFileMetadataID::FModioFileMetadataID() : FileMetadataID(INDEX_NONE) {}

FModioUserID::FModioUserID() : UserID(INDEX_NONE) {}

FModioApiKey::FModioApiKey(const FString& InApiKey) : ApiKey(TCHAR_TO_UTF8(*InApiKey)) {}
FModioApiKey::FModioApiKey(const Modio::ApiKey& InApiKey) : ApiKey(TCHAR_TO_UTF8((*InApiKey).c_str())) {}

FModioEmailAddress::FModioEmailAddress(const FString& InEmailAddress) : EmailAddress(InEmailAddress.TrimStartAndEnd())
{}

FModioEmailAuthCode::FModioEmailAuthCode(const FString& InEmailAuthCode)
	: EmailAuthCode(InEmailAuthCode.TrimStartAndEnd())
{}
