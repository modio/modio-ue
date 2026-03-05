#include "Internal/Convert/List.h"
#include "Internal/Convert/UserRatingList.h"
#include "Types/ModioUserModRating.h"

template const TArray<FModioUserModRating>& FModioList<TArray, FModioUserModRating>::GetRawList() const;

FModioUserModRatingList::FModioUserModRatingList(const Modio::UserRatingList& UserRatingList)
	: FModioPagedResult(UserRatingList)
{
	TArray<FModioUserModRating> Result;
	Result.Reserve(size_t(UserRatingList.Size()));
	for (const Modio::UserRating& Rating : UserRatingList)
	{
		Result.Emplace(ToUnreal(Rating));
	}
	InternalList = Result;
}

FModioOptionalUserModRatingList::FModioOptionalUserModRatingList(TOptional<FModioUserModRatingList>&& UserModRatingList)
	: Internal(UserModRatingList)
{}
