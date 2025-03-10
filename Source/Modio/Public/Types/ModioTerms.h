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

#include "Containers/UnrealString.h"

#include "ModioTerms.generated.h"

/**
 * @docpublic
 * @brief Strong type struct to store links and metadata associated with a mod
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioLink
{
	GENERATED_BODY()

	/** 
	 * @docpublic
	 * @brief The user-facing text for the link
	 */
	UPROPERTY(BlueprintReadOnly, Category="Terms|Link")
	FString Text;

	/** 
	 * @docpublic
	 * @brief The actual URL for the link
	 */
	UPROPERTY(BlueprintReadOnly, Category="Terms|Link")
	FString URL;

	/** 
	 * @docpublic
	 * @brief Is displaying this link mandatory?
	 */
	UPROPERTY(BlueprintReadOnly, Category="Terms|Link")
	bool bRequired = false;
};

/**
 * @docpublic
 * @brief Struct with the strings to display to a user when showing the terms of use
 * and the reference URLs to mod.io user account page
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioTerms
{
	GENERATED_BODY()
	;

	/** 
	 * @docpublic
	 * @brief Text to display on the affirmative/OK button
	 */
	UPROPERTY(BlueprintReadOnly, Category="Terms|Buttons")
	FString AgreeButtonText;

	/** 
	 * @docpublic
	 * @brief Text to display on the negative/cancel button
	 */
	UPROPERTY(BlueprintReadOnly, Category="Terms|Buttons")
	FString DisagreeButtonText;

	/** 
	 * @docpublic
	 * @brief Link to the mod.io website
	 */
	UPROPERTY(BlueprintReadOnly, Category="Terms|Links")
	FModioLink WebsiteLink;

	/** 
	 * @docpublic
	 * @brief Link to the mod.io terms of use
	 */
	UPROPERTY(BlueprintReadOnly, Category="Terms|Links")
	FModioLink TermsLink;

	/** 
	 * @docpublic
	 * @brief Link to the mod.io Privacy Policy
	 */
	UPROPERTY(BlueprintReadOnly, Category="Terms|Links")
	FModioLink PrivacyLink;

	/** 
	 * @docpublic
	 * @brief Link to the mod.io Refund Policy
	 */
	UPROPERTY(BlueprintReadOnly, Category="Terms|Links")
	FModioLink RefundLink;

	/** 
	 * @docpublic
	 * @brief Link to the mod.io Manage User Account page
	 */
	UPROPERTY(BlueprintReadOnly, Category="Terms|Links")
	FModioLink ManageLink;

	/** 
	 * @docpublic
	 * @brief The plaintext version of the mod.io terms of use
	 */
	UPROPERTY(BlueprintReadOnly, Category="Terms|Links")
	FString TermsText;
};

/**
 * @docpublic
 * @brief Struct to store an optional value for the terms of service
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioOptionalTerms
{
	GENERATED_BODY()

	/** 
	 * @docpublic
	 * @brief The Modio terms as an optional value
	 */
	TOptional<FModioTerms> Internal;
};
