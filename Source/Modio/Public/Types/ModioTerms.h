/* 
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *  
 *  This file is part of the mod.io UE4 Plugin.
 *  
 *  Distributed under the MIT License. (See accompanying file LICENSE or 
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *   
 */

#pragma once

#include "Containers/UnrealString.h"

#include "ModioTerms.generated.h"

USTRUCT(BlueprintType)
struct MODIO_API FModioLink
{
	GENERATED_BODY()
	
	/** @brief The user-facing text for the link */
	UPROPERTY(BlueprintReadOnly,Category="Terms|Link")
	FString Text;
	/** @brief The actual URL for the link */
	UPROPERTY(BlueprintReadOnly,Category="Terms|Link")
	FString URL;
	/** @brief Is displaying this link mandatory? */
	UPROPERTY(BlueprintReadOnly,Category="Terms|Link")
	bool bRequired;
};

/**
 * @brief This struct contains strings that should be displayed to a user
 * when displaying the terms of use and offering to create a mod.io account
 **/
USTRUCT(BlueprintType)
struct MODIO_API FModioTerms
{
	GENERATED_BODY();
	
	/** @brief Text to display on the affirmative/OK button */
	UPROPERTY(BlueprintReadOnly,Category="Terms|Buttons")
	FString AgreeButtonText;

	/** @brief Text to display on the negative/cancel button */
	UPROPERTY(BlueprintReadOnly,Category="Terms|Buttons")
	FString DisagreeButtonText;

	
	/** @brief Link to the mod.io website */
	UPROPERTY(BlueprintReadOnly,Category="Terms|Links")
	FModioLink WebsiteLink;
	
	/** @brief Link to the mod.io terms of use */
	UPROPERTY(BlueprintReadOnly,Category="Terms|Links")
	FModioLink TermsLink;
	
	/** @brief Link to the mod.io Privacy Policy */
	UPROPERTY(BlueprintReadOnly,Category="Terms|Links")
	FModioLink PrivacyLink;
	
	/** @brief Link to the mod.io Manage User Account page */
	UPROPERTY(BlueprintReadOnly,Category="Terms|Links")
	FModioLink ManageLink;

	
	/** @brief The plaintext version of the mod.io terms of use */
	FString TermsText;
};

USTRUCT(BlueprintType)
struct MODIO_API FModioOptionalTerms
{
	GENERATED_BODY()

	TOptional<FModioTerms> Internal;
};
