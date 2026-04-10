/*
 *  Copyright (C) 2026 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "Libraries/ModioUserListLibrary.h"

const TArray<FModioUser>& UModioUserListLibrary::GetUsers(const FModioUserList& UserList)
{
	return UserList.GetRawList();
}

const FModioPagedResult& UModioUserListLibrary::GetPagedResult(const FModioUserList& UserList)
{
	return UserList;
}
