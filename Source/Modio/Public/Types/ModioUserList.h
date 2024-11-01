/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "Types/ModioList.h"
#include "Types/ModioPagedResult.h"
#include "Types/ModioUser.h"

#include "ModioUserList.generated.h"

/**
 * Strong type struct to wrap multiple Users indexed by a paged result
 **/
USTRUCT(BlueprintType)
struct MODIO_API FModioUserList
{
	GENERATED_BODY();

	/**
	 * A paged result property
	 **/
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|UserList")
	FModioPagedResult PagedResult;

	/**
	 * Internal array of users
	 **/
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|UserList")
	TArray<FModioUser> InternalList;
};

/**
 * Struct to wrap UserList into an optional parameter
 **/
USTRUCT(BlueprintType)
struct MODIO_API FModioOptionalUserList
{
	GENERATED_BODY();

	/**
	 * Default constructor without parameters
	 **/
	FModioOptionalUserList() = default;

	/**
	 * Constructor with a user list parameter to initialize an instance
	 * @param UserList Optional value of a FModioUserList
	 **/
	FModioOptionalUserList(TOptional<FModioUserList>&& UserList);

	/**
	 * Stored optional ModioUserList
	 **/
	TOptional<FModioUserList> Internal;
};
