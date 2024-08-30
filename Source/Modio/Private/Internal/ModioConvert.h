/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#pragma once

#include "Containers/StringConv.h"
#include "Containers/UnrealString.h"
#include "Internal/ModioPrivateDefines.h"
#include "Misc/DateTime.h"
#include "ModioSDK.h"
#include "Templates/UnrealTypeTraits.h"
#include "Types/ModioCommonTypes.h"
#include "Types/ModioEntitlementConsumptionStatusList.h"
#include "Types/ModioGameInfoList.h"
#include "Types/ModioModCollectionEntry.h"
#include "Types/ModioModInfoList.h"
#include "Types/ModioModProgressInfo.h"
#include "Types/ModioTerms.h"
#include "Types/ModioTransactionRecord.h"
#include "Types/ModioUser.h"

#include <chrono>
#include <string>
#include <vector>

int64 ToUnreal(std::int64_t Value);
double ToUnreal(double Value);
bool ToUnreal(bool Value);
uint64 ToUnreal(std::size_t Value);
FString ToUnreal(const std::string& String);
FDateTime ToUnrealDateTime(std::int64_t UnixTimestamp);
FText ToUnrealText(const std::string& String);
FString ToUnrealString(EModioLanguage Language);

struct FModioModID ToUnreal(Modio::ModID Value);
struct FModioFileMetadataID ToUnreal(Modio::FileMetadataID Value);
struct FModioModTagInfo ToUnreal(const Modio::ModTagInfo& In);
struct FModioMetadata ToUnreal(const Modio::Metadata& In);
struct FModioModTag ToUnreal(const Modio::ModTag& In);
struct FModioModTagLocalizationData ToUnreal(const Modio::ModTagLocalizationData& In);
struct FModioModInfo ToUnreal(const Modio::ModInfo& In);
struct FModioModDependency ToUnreal(const Modio::ModDependency& In);
struct FModioUnsigned64 ToUnreal(const Modio::FileSize& In);
struct FModioOtherUrl ToUnreal(const Modio::OtherUrl& In);
struct FModioGameInfo ToUnreal(const Modio::GameInfo& In);
struct FModioGameID ToUnreal(Modio::GameID Value);
struct FModioTheme ToUnreal(const Modio::Theme& In);
struct FModioLogo ToUnreal(const Modio::Detail::Logo& In);
struct FModioHeaderImage ToUnreal(const Modio::HeaderImage& In);
struct FModioIcon ToUnreal(const Modio::Detail::Icon& In);
struct FModioGameStats ToUnreal(const Modio::GameStats& In);
EModioModfilePlatform ToUnreal(const Modio::ModfilePlatform& In);
struct FModioGamePlatform ToUnreal(const Modio::GamePlatform& In);
EModioModChangeType ToUnreal(const Modio::UserSubscriptionList::ChangeType& In);
EModioLanguage ToUnreal(const Modio::Language& In);
FEntitlementConsumptionStatus ToUnreal(const Modio::EntitlementConsumptionStatus& In);
FModioEntitlementConsumptionVirtualCurrencyDetails ToUnreal(const Modio::EntitlementConsumptionVirtualCurrencyDetails& In);
FModioEntitlementWalletBalance ToUnreal(const Modio::EntitlementWalletBalance& In);
EModioEntitlementConsumptionState ToUnreal(const Modio::EntitlementConsumptionState& In);
EModioEntitlementType ToUnreal(const Modio::EntitlementType& In);
FModioGameInfoList ToUnreal(const Modio::GameInfoList& In);
FModioUser ToUnreal(const Modio::User& In);
FModioModInfoList ToUnreal(const Modio::ModInfoList& In);
FModioModTagOptions ToUnreal(const Modio::ModTagOptions& In);
FModioLink ToUnreal(const Modio::Terms::Link& In);
FModioTerms ToUnreal(const Modio::Terms& In);
FModioTransactionRecord ToUnreal(const Modio::TransactionRecord& In);
FModioModProgressInfo ToUnreal(const Modio::ModProgressInfo& In);
EModioModProgressState ToUnreal(const Modio::ModProgressInfo::EModProgressState& In);
FModioModCollectionEntry ToUnreal(const Modio::ModCollectionEntry& In);

std::string ToModio(const FString& String);
std::vector<std::string> ToModio(const TArray<FString>& StringArray);
std::vector<Modio::ModID> ToModio(const TArray<FModioModID>& ModIDArray);
std::map<std::string, std::string> ToModio(const TMap<FString, FString>& StringMap);
std::chrono::system_clock::time_point ToModioDateTime(FDateTime Time);
Modio::ApiKey ToModio(const FModioApiKey& In);
Modio::EmailAddress ToModio(const FModioEmailAddress& In);
Modio::EmailAuthCode ToModio(const FModioEmailAuthCode& In);
Modio::ModID ToModio(const FModioModID& In);
Modio::GameID ToModio(const FModioGameID& In);
Modio::FileMetadataID ToModio(const FModioFileMetadataID& In);
Modio::UserID ToModio(const FModioUserID& In);
Modio::LogLevel ToModio(EModioLogLevel UnrealLogLevel);
Modio::AvatarSize ToModio(EModioAvatarSize AvatarSize);
Modio::GallerySize ToModio(EModioGallerySize GallerySize);
Modio::LogoSize ToModio(EModioLogoSize LogoSize);
Modio::Environment ToModio(EModioEnvironment Environment);
Modio::ModfilePlatform ToModio(EModioModfilePlatform Platform);
Modio::Portal ToModio(EModioPortal Portal);
Modio::Language ToModio(EModioLanguage Language);
Modio::EntitlementParams ToModio(const FModioEntitlementParams& InParams);

#pragma region ToModio implementation

MODIO_BEGIN_CONVERT_SWITCHES

MODIO_END_CONVERT_SWITCHES

template<typename DestType, typename UnderlyingType, typename FlagType>
typename TEnableIf<TIsDerivedFrom<FlagType, Modio::FlagImpl<UnderlyingType>>::Value, DestType>::Type ToUnreal(
	FlagType Flags)
{
	return static_cast<DestType>(Flags.RawValue());
}

template<typename DestValueType, typename SourceValueType, typename... OtherParams>
TArray<DestValueType> ToUnreal(std::vector<SourceValueType, OtherParams...>&& OriginalArray)
{
	TArray<DestValueType> Result;

	Result.Reserve(OriginalArray.size());
	for (auto& It : OriginalArray)
	{
		Result.Emplace(ToUnreal(MoveTemp(It)));
	}

	return Result;
}

template<typename DestValueType, typename SourceValueType, typename... OtherParams>
TArray<DestValueType> ToUnreal(const std::vector<SourceValueType, OtherParams...>& OriginalArray)
{
	TArray<DestValueType> Result;

	Result.Reserve(OriginalArray.size());
	for (const auto& It : OriginalArray)
	{
		Result.Emplace(ToUnreal(It));
	}

	return Result;
}

template<typename DestKeyType, typename DestValueType, typename SourceKeyType, typename SourceValueType,
		 typename... OtherParams>
TMap<DestKeyType, DestValueType> ToUnreal(const std::map<SourceKeyType, SourceValueType, OtherParams...>& OriginalMap)
{
	TMap<DestKeyType, DestValueType> Result;
	Result.Reserve(OriginalMap.size());

	for (const auto& [Key, Value] : OriginalMap)
	{
		Result.Add(ToUnreal(Key), ToUnreal(Value));
	}

	return Result;
}

template<typename DestValueType, typename SourceValueType>
TOptional<DestValueType> ToUnrealOptional(SourceValueType&& Original)
{
	TOptional<DestValueType> DestinationOptional = {};
	if (Original)
	{
		DestinationOptional = ToUnreal(Original.value());
	}

	return DestinationOptional;
}

template<typename DestValueType, typename SourceValueType>
DestValueType ToBP(SourceValueType&& Original)
{
	DestValueType Result = {MoveTempIfPossible(Original)};
	return Result;
}

template<typename DestValueType, typename SourceValueType>
std::vector<DestValueType> ToModio(const TArray<SourceValueType>& OriginalArray)
{
	std::vector<DestValueType> Out;
	Out.reserve(OriginalArray.Num());
	for (const SourceValueType& Element : OriginalArray)
	{
		Out.emplace_back(ToModio(Element));
	}
	return Out;
}

#pragma endregion
