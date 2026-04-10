/*
 *  Copyright (C) 2026 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Types/ModioUserList.h"

#include "ModioUserListLibrary.generated.h"

/**
 * @docpublic
 * @brief Utility functions for accessing and managing lists of user information
 */
UCLASS()
class MODIO_API UModioUserListLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	/**
	 * @docpublic
	 * @brief Retrieve the list of users from the user list
	 * @param UserList The user list to retrieve the Users from
	 * @return An array of FModioUser objects representing the Users in the list
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities")
	static const TArray<FModioUser>& GetUsers(const FModioUserList& UserList);

	/**
	 * @docpublic
	 * @brief Retrieve paged result information for a user list
	 * @param UserList The user list to retrieve paged result information from
	 * @return An FModioPagedResult containing metadata about the paginated data
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities")
	static const FModioPagedResult& GetPagedResult(const FModioUserList& UserList);

};
