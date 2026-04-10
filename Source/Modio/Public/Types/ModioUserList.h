/*
 *  Copyright (C) 2024-2026 mod.io Pty Ltd. <https://mod.io>
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

namespace Modio
{
	class UserList;
}

#if CPP
struct MODIO_API FModioUserList : public FModioPagedResult, public FModioList<TArray, FModioUser>
{
	/**
	 * @docpublic
	 * @brief Default constructor without parameters
	 */
	FModioUserList() = default;

	/**
	 * @docpublic
	 * @brief Constructor that takes an array of users and a index of paged results
	 */
	FModioUserList(const FModioPagedResult& PagedResult, TArray<FModioUser>&& UserList);

	/**
	 * @docpublic
	 * @brief Constructor that takes a user list to store in this struct
	 */
	FModioUserList(const class Modio::UserList& UserList);
};
#else
/**
 * @docpublic
 * @brief Strong type struct to wrap multiple Users indexed by a paged result
 */
USTRUCT(NoExport, BlueprintType)
struct MODIO_API FModioUserList
{
	GENERATED_BODY()
	;

	/** 
	 * @docpublic
	 * @brief A paged result property
	 */
	UPROPERTY()
	FModioPagedResult PagedResult;

	/** 
	 * @docpublic
	 * @brief Internal array of users
	 */
	UPROPERTY()
	TArray<FModioUser> InternalList;
};
#endif

/**
 * @docpublic
 * @brief Struct to wrap UserList into an optional parameter
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioOptionalUserList
{
	GENERATED_BODY()
	;

	/** 
	 * @docpublic
	 * @brief Default constructor without parameters
	 */
	FModioOptionalUserList() = default;

	/** 
	 * @docpublic
	 * @brief Constructor with a user list parameter to initialize an instance
	 * @param UserList Optional value of a FModioUserList
	 */
	FModioOptionalUserList(TOptional<FModioUserList>&& UserList);

	/** 
	 * @docpublic
	 * @brief Stored optional ModioUserList
	 */
	TOptional<FModioUserList> Internal;
};
