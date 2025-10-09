/*
 *  Copyright (C) 2024-2025 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "Libraries/ModioCommonTypesLibrary.h"
#include "Types/ModioAuthenticationParams.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioCommonTypesLibrary)

FModioGameID UModioCommonTypesLibrary::MakeGameId(int64 GameId)
{
	return FModioGameID(GameId);
}

FString UModioCommonTypesLibrary::Conv_GameIDToString(FModioGameID GameId)
{
	return GameId.ToString();
}

FModioAuthenticationParams UModioCommonTypesLibrary::MakeAuthParams(const FString AuthToken, const FString EmailAddress,
                                                                    const bool bHasAcceptedTOS)
{
	return FModioAuthenticationParams{AuthToken, EmailAddress, bHasAcceptedTOS};
}

FModioApiKey UModioCommonTypesLibrary::MakeApiKey(const FString ApiKey)
{
	return FModioApiKey(ApiKey);
}

FString UModioCommonTypesLibrary::Conv_ApiKeyToString(FModioApiKey ApiKey)
{
	return ApiKey.ToString();
}

FModioEmailAddress UModioCommonTypesLibrary::Conv_StringToEmailAddress(const FString& Email)
{
	return FModioEmailAddress(Email);
}

FModioEmailAuthCode UModioCommonTypesLibrary::Conv_StringToEmailAuthCode(const FString& AuthCode)
{
	return FModioEmailAuthCode(AuthCode);
}

FString UModioCommonTypesLibrary::Conv_EmailAddressToString(FModioEmailAddress EmailAddress)
{
	return EmailAddress.ToString();
}

FString UModioCommonTypesLibrary::Conv_EmailAuthCodeToString(FModioEmailAuthCode EmailAuthCode)
{
	return EmailAuthCode.ToString();
}

FString UModioCommonTypesLibrary::Conv_FileMetadataIDToString(FModioFileMetadataID FileMetadataID)
{
	return FileMetadataID.ToString();
}

FString UModioCommonTypesLibrary::Conv_ModIDToString(FModioModID ModID)
{
	return ModID.ToString();
}

FString UModioCommonTypesLibrary::Conv_UserIDToString(FModioUserID UserID)
{
	return UserID.ToString();
}

FModioInitializeOptions UModioCommonTypesLibrary::MakeInitializeOptions(int64 GameId, const FString& APIKey,
                                                                        EModioEnvironment GameEnvironment,
                                                                        EModioPortal PortalInUse,
                                                                        bool bUseBackgroundThread)
{
	FModioInitializeOptions Options;
	Options.GameId = FModioGameID(GameId);
	Options.ApiKey = FModioApiKey(APIKey);
	Options.GameEnvironment = GameEnvironment;
	Options.PortalInUse = PortalInUse;
	Options.bUseBackgroundThread = bUseBackgroundThread;
	return Options;
}

FModioInitializeOptions UModioCommonTypesLibrary::SetGameId(const FModioInitializeOptions& Options, int64 GameId)
{
	FModioInitializeOptions DuplicateOptions{Options};
	DuplicateOptions.GameId = FModioGameID(GameId);
	return DuplicateOptions;
}

FModioInitializeOptions UModioCommonTypesLibrary::SetAPIKey(const FModioInitializeOptions& Options,
                                                            const FString& APIKey)
{
	FModioInitializeOptions DuplicateOptions{Options};
	DuplicateOptions.ApiKey = FModioApiKey(APIKey);
	return DuplicateOptions;
}

FModioInitializeOptions UModioCommonTypesLibrary::SetGameEnvironment(const FModioInitializeOptions& Options,
                                                                     EModioEnvironment GameEnvironment)
{
	FModioInitializeOptions DuplicateOptions{Options};
	DuplicateOptions.GameEnvironment = GameEnvironment;
	return DuplicateOptions;
}

FModioInitializeOptions UModioCommonTypesLibrary::SetPortal(const FModioInitializeOptions& Options,
                                                            EModioPortal PortalToUse)
{
	FModioInitializeOptions DuplicateOptions{Options};
	DuplicateOptions.PortalInUse = PortalToUse;
	return DuplicateOptions;
}

FModioInitializeOptions UModioCommonTypesLibrary::SetBackgroundThread(const FModioInitializeOptions& Options,
                                                                      bool bUseBackgroundThread)
{
	FModioInitializeOptions DuplicateOptions{Options};
	DuplicateOptions.bUseBackgroundThread = bUseBackgroundThread;
	return DuplicateOptions;
}

FModioInitializeOptions UModioCommonTypesLibrary::SetSessionIdentifier(const FModioInitializeOptions& Options,
                                                                       const FString& SessionIdentifier)
{
	FModioInitializeOptions DuplicateOptions{Options};
	DuplicateOptions.LocalSessionIdentifier = SessionIdentifier;
	return DuplicateOptions;
}

FModioInitializeOptions UModioCommonTypesLibrary::SetExtendedInitializationParameters(
	const FModioInitializeOptions& Options, const TMap<FString, FString>& ExtendedParameters)
{
	FModioInitializeOptions DuplicateOptions{Options};
	DuplicateOptions.ExtendedInitializationParameters = ExtendedParameters;
	return DuplicateOptions;
}

int64 UModioCommonTypesLibrary::GetRawValueFromModID(const FModioModID& In)
{
	return GetUnderlyingValue(In);
}

int64 UModioCommonTypesLibrary::GetRawValueFromModCollectionID(const FModioModCollectionID& In)
{
	return GetUnderlyingValue(In);
}

bool UModioCommonTypesLibrary::EqualTo(const FModioModID& A, const FModioModID& B)
{
	return A == B;
}

bool UModioCommonTypesLibrary::EqualToModCollectionID(const FModioModCollectionID& A, const FModioModCollectionID& B)
{
	return A == B;
}

bool UModioCommonTypesLibrary::NotEqualTo(const FModioModID& A, const FModioModID& B)
{
	return A != B;
}

FModioEntitlementParams UModioCommonTypesLibrary::MakeEntitlementParams(
	const TMap<FString, FString>& ExtendedParameters)
{
	return FModioEntitlementParams(ExtendedParameters);
}

FModioMetricsSessionParams UModioCommonTypesLibrary::MakeMetricsSessionParams(const TArray<FModioModID>& Ids)
{
	return FModioMetricsSessionParams(Ids);
}

FModioMetricsSessionParams UModioCommonTypesLibrary::SetSessionId(const FModioMetricsSessionParams& Params,
                                                                  const FModioGuid& Id)
{
	FModioMetricsSessionParams DuplicateOptions{Params};
	DuplicateOptions.SessionId = FModioGuid(Id);
	return DuplicateOptions;
}

FModioGuid UModioCommonTypesLibrary::MakeGuid(const FString Guid)
{
	return FModioGuid(Guid);
}