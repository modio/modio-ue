#include "Libraries/ModioCommonTypesLibrary.h"
#include "Types/ModioAuthenticationParams.h"

FModioGameID UModioCommonTypesLibrary::MakeGameId(int64 GameId)
{
	return FModioGameID(Modio::GameID(GameId));
}

FString UModioCommonTypesLibrary::Conv_GameIDToString(FModioGameID GameId)
{
	return GameId.ToString();
}

FModioAuthenticationParams UModioCommonTypesLibrary::MakeAuthParams(const FString AuthToken,
																		   const FString EmailAddress,
																		   const bool bHasAcceptedTOS)
{
	return FModioAuthenticationParams { AuthToken, EmailAddress, bHasAcceptedTOS };
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

FModioInitializeOptions UModioCommonTypesLibrary::MakeInitializeOptions(int64 GameId, const FString& APIKey, EModioEnvironment GameEnvironment, EModioPortal PortalInUse)
{
	FModioInitializeOptions Options;
	Options.GameID = FModioGameID(GameId);
	Options.APIKey = FModioApiKey(APIKey);
	Options.GameEnvironment = GameEnvironment;
	Options.PortalInUse = PortalInUse;
	return Options;
}

FModioInitializeOptions UModioCommonTypesLibrary::SetPortal(const FModioInitializeOptions& Options,
															EModioPortal PortalToUse)
{
	FModioInitializeOptions DuplicateOptions {Options};
	DuplicateOptions.PortalInUse = PortalToUse;
	return DuplicateOptions;
}

