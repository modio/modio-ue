/*
 *  Copyright (C) 2024-2025 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "Libraries/ModioOptionalLibrary.h"
#include "Types/ModioGameInfo.h"
#include "Types/ModioGameInfoList.h"
#include "Types/ModioModDependencyList.h"
#include "Types/ModioModInfoList.h"
#include "Types/ModioModTagOptions.h"
#include "Types/ModioUser.h"
#include "Types/ModioModChangeMap.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioOptionalLibrary)

template<typename BPOptionalType>
bool IsSetInternal(const BPOptionalType& Optional)
{
	return Optional.Internal.IsSet();
}

template<typename BPOptionalType, typename Type>
bool GetValueInternal(const BPOptionalType& BPOptional, Type& OutValue)
{
	if (BPOptional.Internal.IsSet())
	{
		OutValue = BPOptional.Internal.GetValue();
		return true;
	}

	return false;
}

bool UModioOptionalLibrary::IsSet_ModioOptionalModInfoList(const FModioOptionalModInfoList& OptionalModInfoList)
{
	return IsSetInternal(OptionalModInfoList);
}

bool UModioOptionalLibrary::GetValue_ModioOptionalModInfoList(const FModioOptionalModInfoList& OptionalModInfoList,
															  FModioModInfoList& ModInfoList)
{
	return GetValueInternal(OptionalModInfoList, ModInfoList);
}

bool UModioOptionalLibrary::IsSet_ModioOptionalGameInfoList(const FModioOptionalGameInfoList& OptionalGameInfoList)
{
	return IsSetInternal(OptionalGameInfoList);
}

bool UModioOptionalLibrary::GetValue_ModioOptionalGameInfoList(const FModioOptionalGameInfoList& OptionalGameInfoList,
															   FModioGameInfoList& GameInfoList)
{
	return GetValueInternal(OptionalGameInfoList, GameInfoList);
}

bool UModioOptionalLibrary::IsSet_ModioOptionalModInfo(const FModioOptionalModInfo& OptionalModInfo)
{
	return IsSetInternal(OptionalModInfo);
}

bool UModioOptionalLibrary::GetValue_ModioOptionalModInfo(const FModioOptionalModInfo& OptionalModInfo,
														  FModioModInfo& ModInfo)
{
	return GetValueInternal(OptionalModInfo, ModInfo);
}

bool UModioOptionalLibrary::IsSet_ModioOptionalGameInfo(const FModioOptionalGameInfo& OptionalGameInfo)
{
	return IsSetInternal(OptionalGameInfo);
}

bool UModioOptionalLibrary::GetValue_ModioOptionalGameInfo(const FModioOptionalGameInfo& OptionalGameInfo,
														   FModioGameInfo& GameInfo)
{
	return GetValueInternal(OptionalGameInfo, GameInfo);
}

bool UModioOptionalLibrary::IsSet_ModioOptionalUser(const struct FModioOptionalUser& OptionalUser)
{
	return IsSetInternal(OptionalUser);
}

bool UModioOptionalLibrary::GetValue_ModioOptionalUser(const struct FModioOptionalUser& OptionalUser,
													   struct FModioUser& User)
{
	return GetValueInternal(OptionalUser, User);
}

bool UModioOptionalLibrary::IsSet_ModioOptionalModTagOptions(
	const struct FModioOptionalModTagOptions& OptionalModTagOptions)
{
	return IsSetInternal(OptionalModTagOptions);
}

bool UModioOptionalLibrary::GetValue_ModioOptionalModTagOptions(
	const struct FModioOptionalModTagOptions& OptionalModTagOptions, struct FModioModTagOptions& ModTagOptions)
{
	return GetValueInternal(OptionalModTagOptions, ModTagOptions);
}

bool UModioOptionalLibrary::IsSet_ModioOptionalModProgressInfo(
	const struct FModioOptionalModProgressInfo& OptionalModProgressInfo)
{
	return IsSetInternal(OptionalModProgressInfo);
}

bool UModioOptionalLibrary::GetValue_ModioOptionalModProgressInfo(
	const struct FModioOptionalModProgressInfo& OptionalModProgressInfo, struct FModioModProgressInfo& ModProgressInfo)
{
	return GetValueInternal(OptionalModProgressInfo, ModProgressInfo);
}

bool UModioOptionalLibrary::IsSet_ModioOptionalImage(const struct FModioOptionalImage& OptionalImage)
{
	return IsSetInternal(OptionalImage);
}

bool UModioOptionalLibrary::GetValue_ModioOptionalImage(const struct FModioOptionalImage& OptionalImage,
														struct FModioImageWrapper& Image)
{
	return GetValueInternal(OptionalImage, Image);
}

bool UModioOptionalLibrary::IsSet_ModioOptionalTerms(const FModioOptionalTerms& OptionalTerms)
{
	return IsSetInternal(OptionalTerms);
}

bool UModioOptionalLibrary::GetValue_ModioOptionalTerms(const FModioOptionalTerms& OptionalTerms, FModioTerms& Terms)
{
	return GetValueInternal(OptionalTerms, Terms);
}

bool UModioOptionalLibrary::IsSet_ModioOptionalModDependencyList(
	const struct FModioOptionalModDependencyList& OptionalDependencyList)
{
	return IsSetInternal(OptionalDependencyList);
}

bool UModioOptionalLibrary::GetValue_ModioOptionalModDependencyList(
	const struct FModioOptionalModDependencyList& OptionalDependencyList,
	struct FModioModDependencyList& DependencyList)
{
	return GetValueInternal(OptionalDependencyList, DependencyList);
}

bool UModioOptionalLibrary::IsSet_ModioOptionalModID(const struct FModioOptionalModID& OptionalID)
{
	return IsSetInternal(OptionalID);
}

bool UModioOptionalLibrary::GetValue_ModioOptionalModID(const struct FModioOptionalModID& OptionalID,
														struct FModioModID& ID)
{
	return GetValueInternal(OptionalID, ID);
}

bool UModioOptionalLibrary::IsSet_ModioOptionalUInt64(const struct FModioOptionalUInt64& OptionalInt)
{
	return IsSetInternal(OptionalInt);
}

bool UModioOptionalLibrary::GetValue_ModioOptionalUInt64(const struct FModioOptionalUInt64& OptionalInt,
														 FModioUnsigned64& UnderlyingInt)
{
	// FModioOptionalUInt64 uses TOptional<uint64_t> internally rather than FModioUnsigned64, so we're not using the
	// GetValueInternal() template function here
	if (OptionalInt.Internal.IsSet())
	{
		UnderlyingInt = FModioUnsigned64(OptionalInt.Internal.GetValue());
		return true;
	}
	return false;
}

bool UModioOptionalLibrary::IsSet_ModioOptionalModChangeMap(const struct FModioOptionalModChangeMap& OptionalModChangeMap)
{
	return IsSetInternal(OptionalModChangeMap);
}

bool UModioOptionalLibrary::GetValue_ModioOptionalModChangeMap(const struct FModioOptionalModChangeMap& OptionalModChangeMap, struct FModioModChangeMap& ModChangeMap)
{
	return GetValueInternal(OptionalModChangeMap, ModChangeMap);
}

bool UModioOptionalLibrary::IsSet_ModioOptionalModCollectionInfo(
	const FModioOptionalModCollectionInfo& OptionalModCollectionInfo)
{
	return IsSetInternal(OptionalModCollectionInfo);
}

bool UModioOptionalLibrary::GetValue_ModioOptionalModCollectionInfo(
	const FModioOptionalModCollectionInfo& OptionalModCollectionInfo, FModioModCollectionInfo& ModCollectionInfo)
{
	return GetValueInternal(OptionalModCollectionInfo, ModCollectionInfo);
}

bool UModioOptionalLibrary::IsSet_ModioOptionalModCollectionInfoList(
	const FModioOptionalModCollectionInfoList& OptionalModCollectionInfoList)
{
	return IsSetInternal(OptionalModCollectionInfoList);
}

bool UModioOptionalLibrary::GetValue_ModioOptionalModCollectionInfoList(
	const FModioOptionalModCollectionInfoList& OptionalModCollectionInfoList,
	FModioModCollectionInfoList& ModCollectionInfoList)
{
	return GetValueInternal(OptionalModCollectionInfoList, ModCollectionInfoList);
}
