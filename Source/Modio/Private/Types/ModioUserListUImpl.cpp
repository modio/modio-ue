/*
 *  Copyright (C) 2024-2026 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */
#include "Types/ModioUserList.h"

#include "Internal/Convert/User.h"

#include "Internal/Convert/List.h"
#include "ModioSDK.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioUserList)

template const TArray<FModioUser>& FModioList<TArray, FModioUser>::GetRawList() const;

FModioOptionalUserList::FModioOptionalUserList(TOptional<FModioUserList>&& ModDependencies)
	: Internal(MoveTemp(ModDependencies))
{}

FModioUserList::FModioUserList(const Modio::UserList& UserList) : FModioPagedResult(UserList)
{
	TArray<FModioUser> Result;

	Result.Reserve(size_t(UserList.Size()));
	for (const Modio::User& It : UserList)
	{
		Result.Emplace(ToUnreal(It));
	}

	InternalList = Result;
}

FModioUserList::FModioUserList(const FModioPagedResult& PagedResult, TArray<FModioUser>&& UserList)
	: FModioPagedResult(PagedResult),
	  FModioList(MoveTemp(UserList))
{}