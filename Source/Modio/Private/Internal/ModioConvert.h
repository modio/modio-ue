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
#include <chrono>
#include <string>
#include <vector>



int64 ToUnreal(std::int64_t Value);
double ToUnreal(double Value);
bool ToUnreal(bool Value);
uint64 ToUnreal(std::size_t Value);
FString ToUnreal(const std::string& String);
FString ToUnreal(const Modio::filesystem::path& Path);
FDateTime ToUnrealDateTime(std::int64_t UnixTimestamp);

struct FModioModID ToUnreal(Modio::ModID Value);
struct FModioFileMetadataID ToUnreal(Modio::FileMetadataID Value);
struct FModioModTagInfo ToUnreal(const Modio::ModTagInfo& In);
struct FModioMetadata ToUnreal(const Modio::Metadata& In);
struct FModioModTag ToUnreal(const Modio::ModTag& In);
struct FModioModInfo ToUnreal(const Modio::ModInfo& In);
struct FModioModDependency ToUnreal(const Modio::ModDependency& In);

std::string ToModio(const FString& String);
std::vector<std::string> ToModio(const TArray<FString>& StringArray);
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
Modio::Portal ToModio(EModioPortal Portal);
Modio::Language ToModio(EModioLanguage Language);

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
