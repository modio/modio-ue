/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "Libraries/ModioOptionalLibrary.h"
#include "Types/ModioModInfoList.h"
#include "Types/ModioModTagOptions.h"
#include "Types/ModioUser.h"
#include "Types/ModioModDependencyList.h"

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

bool UModioOptionalLibrary::IsSet_ModioOptionalModInfo(const FModioOptionalModInfo& OptionalModInfo)
{
	return IsSetInternal(OptionalModInfo);
}

bool UModioOptionalLibrary::GetValue_ModioOptionalModInfo(const FModioOptionalModInfo& OptionalModInfo,
														  FModioModInfo& ModInfo)
{
	return GetValueInternal(OptionalModInfo, ModInfo);
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
