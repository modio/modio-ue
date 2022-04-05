/* 
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *  
 *  This file is part of the mod.io UE4 Plugin.
 *  
 *  Distributed under the MIT License. (See accompanying file LICENSE or 
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *   
 */

#include "Types/ModioCommonTypes.h"
#include "ModioSDK.h"

#include "Internal/ModioConvert.h"

FModioModID::FModioModID() : ModID(INDEX_NONE) {}

uint32 GetTypeHash(FModioModID ModioModId)
{
	return FCrc::MemCrc32(&ModioModId.ModID, sizeof(Modio::ModID));
}

FModioGameID::FModioGameID() : GameId(INDEX_NONE) {}

FModioGameID FModioGameID::InvalidGameID()
{
	return FModioGameID(Modio::GameID::InvalidGameID());
}

uint32 GetTypeHash(FModioGameID ModioGameId)
{
	return FCrc::MemCrc32(&ModioGameId.GameId, sizeof(Modio::GameID));
}

FModioFileMetadataID::FModioFileMetadataID() : FileMetadataID(INDEX_NONE) {}

uint32 GetTypeHash(FModioFileMetadataID FileMetadataID)
{
	return FCrc::MemCrc32(&FileMetadataID.FileMetadataID, sizeof(Modio::FileMetadataID));
}

FModioUserID::FModioUserID() : UserID(INDEX_NONE) {}

uint32 GetTypeHash(FModioUserID UserID)
{
	return FCrc::MemCrc32(&UserID.UserID, sizeof(Modio::UserID));
}

FModioApiKey::FModioApiKey(const FString& InApiKey) : ApiKey(TCHAR_TO_UTF8(*InApiKey)) {}

FModioApiKey FModioApiKey::InvalidAPIKey()
{
	return FModioApiKey(ToUnreal(*Modio::ApiKey::InvalidAPIKey()));
}

FModioEmailAddress::FModioEmailAddress(const FString& InEmailAddress) : EmailAddress(InEmailAddress.TrimStartAndEnd())
{}

FModioEmailAuthCode::FModioEmailAuthCode(const FString& InEmailAuthCode)
	: EmailAuthCode(InEmailAuthCode.TrimStartAndEnd())
{}
