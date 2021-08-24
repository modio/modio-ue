#include "Libraries/ModioOptionalLibrary.h"
#include "Types/ModioModInfoList.h"
#include "Types/ModioModTagOptions.h"
#include "Types/ModioUser.h"

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
														struct FModioImage& Image)
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
