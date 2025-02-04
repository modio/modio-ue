/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#pragma once

#include "Types/ModioUnsigned64.h"
#include "Types/ModioCommonTypes.h"

#include "Interfaces/OnlineStoreInterfaceV2.h"

#include "ModioTokenPack.generated.h"

/**
 * @docpublic
 * @brief A wrapper type around the Unreal Engine type FOnlineStoreOffer, representing a platform-agnostic store offer/product, e.g a Premium Currency pack.
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioTokenPack
{
	GENERATED_BODY()

private:
	FOnlineStoreOffer Underlying;

public:
	/** 
	 * @docpublic
	 * @brief Default constructor for FModioTokenPack
	 */
	FModioTokenPack() = default;

	/** 
	 * @docpublic
	 * @brief Constructs a FModioTokenPack from an existing FOnlineStoreOffer
	 * @param InOffer The store offer to wrap
	 */
	FModioTokenPack(const FOnlineStoreOffer& InOffer)
	{
		Underlying = InOffer;
	}

	/** 
	 * @docpublic
	 * @brief Get the title of the token pack
	 * @return The title text of the token pack
	 */
	FText GetTitle() const
	{
		return Underlying.Title;
	}

	/** 
	 * @docpublic
	 * @brief Get the description of the token pack
	 * @return The description text of the token pack
	 */
	FText GetDescription() const
	{
		return Underlying.Description;
	}

	/** 
	 * @docpublic
	 * @brief Get the long description of the token pack
	 * @return The long description text of the token pack
	 */
	FText GetLongDescription() const
	{
		return Underlying.LongDescription;
	}

	/** 
	 * @docpublic
	 * @brief Get the regular price of the token pack
	 * @return The regular price text of the token pack
	 */
	FText GetRegularPrice() const
	{
		return Underlying.GetDisplayRegularPrice();
	}

	/** 
	 * @docpublic
	 * @brief Get the display price of the token pack
	 * @return The display price text of the token pack
	 */
	FText GetDisplayPrice() const
	{
		return Underlying.GetDisplayPrice();
	}

	/** 
	 * @docpublic
	 * @brief Get the numeric price of the token pack
	 * @return The numeric price of the token pack
	 */
	int64 GetNumericPrice() const
	{
		return Underlying.NumericPrice;
	}

	/** 
	 * @docpublic
	 * @brief Get the currency code of the token pack
	 * @return The currency code of the token pack
	 */
	FString GetCurrencyCode() const
	{
		return Underlying.CurrencyCode;
	}

	/** 
	 * @docpublic
	 * @brief Get the release date of the token pack
	 * @return The release date of the token pack
	 */
	FDateTime GetReleaseDate() const
	{
		return Underlying.ReleaseDate;
	}

	/** 
	 * @docpublic
	 * @brief Get the expiration date of the token pack
	 * @return The expiration date of the token pack
	 */
	FDateTime GetExpirationDate() const
	{
		return Underlying.ExpirationDate;
	}

	/** 
	 * @docpublic
	 * @brief Check if the token pack is purchasable
	 * @return True if the token pack is purchasable, false otherwise
	 */
	bool IsPurchasable() const
	{
		return Underlying.IsPurchaseable();
	}

	/** 
	 * @docpublic
	 * @brief Get any dynamic fields associated with the token pack
	 * @return A map of dynamic fields for the token pack
	 */
	TMap<FString, FString> GetFields() const
	{
		return Underlying.DynamicFields;
	}

	/** 
	 * @docpublic
	 * @brief Get the ID of the token pack
	 * @return The ID of the token pack
	 */
	FString GetId() const
	{
		return Underlying.OfferId;
	}

	/** 
	 * @docpublic
	 * @brief Get the Modio ID for this token pack
	 * @return The Modio ID of the token pack
	 */
	FModioTokenPackID GetModioId() const
	{
		return FModioTokenPackID(Underlying.OfferId);
	}
};

/**
 * @docpublic
 * @brief Strong type struct to store an optional ModInfo parameter
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioOptionalTokenPack
{
	GENERATED_BODY()

	/**
	 * @docpublic
	 * @brief Stored property to an optional ModInfo
	 */
	TOptional<FModioTokenPack> Internal;
};

/**
 * @docpublic
 * @brief Blueprint function library for interacting with ModioTokenPack objects
 */
UCLASS()
class UModioTokenPackLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** 
	 * @docpublic
	 * @brief Get the title of the token pack
	 * @param In The token pack to retrieve the title from
	 * @return The title text of the token pack
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Meta = (Category = "Token Pack"))
	static FText GetTitle(const FModioTokenPack& In)
	{
		return In.GetTitle();
	}

	/** 
	 * @docpublic
	 * @brief Get the description of the token pack
	 * @param In The token pack to retrieve the description from
	 * @return The description text of the token pack
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Meta = (Category = "Token Pack"))
	static FText GetDescription(const FModioTokenPack& In)
	{
		return In.GetDescription();
	}

	/** 
	 * @docpublic
	 * @brief Get the long description of the token pack
	 * @param In The token pack to retrieve the long description from
	 * @return The long description text of the token pack
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Meta = (Category = "Token Pack"))
	static FText GetLongDescription(const FModioTokenPack& In)
	{
		return In.GetLongDescription();
	}

	/** 
	 * @docpublic
	 * @brief Get the regular price of the token pack
	 * @param In The token pack to retrieve the regular price from
	 * @return The regular price text of the token pack
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Meta = (Category = "Token Pack"))
	static FText GetRegularPrice(const FModioTokenPack& In)
	{
		return In.GetRegularPrice();
	}

	/** 
	 * @docpublic
	 * @brief Get the display price of the token pack
	 * @param In The token pack to retrieve the display price from
	 * @return The display price text of the token pack
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Meta = (Category = "Token Pack"))
	static FText GetDisplayPrice(const FModioTokenPack& In)
	{
		return In.GetDisplayPrice();
	}

	/** 
	 * @docpublic
	 * @brief Get the numeric price of the token pack
	 * @param In The token pack to retrieve the numeric price from
	 * @return The numeric price of the token pack
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Meta = (Category = "Token Pack"))
	static int64 GetNumericPrice(const FModioTokenPack& In)
	{
		return In.GetNumericPrice();
	}

	/** 
	 * @docpublic
	 * @brief Check if the token pack is purchasable
	 * @param In The token pack to check if it is purchasable
	 * @return True if the token pack is purchasable, false otherwise
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Meta = (Category = "Token Pack"))
	static bool IsPurchasable(const FModioTokenPack& In)
	{
		return In.IsPurchasable();
	}

	/** 
	 * @docpublic
	 * @brief Get any dynamic fields associated with the token pack
	 * @param In The token pack to retrieve dynamic fields from
	 * @return A map of dynamic fields for the token pack
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Meta = (Category = "Token Pack"))
	static TMap<FString, FString> GetFields(const FModioTokenPack& In)
	{
		return In.GetFields();
	}

	/** 
	 * @docpublic
	 * @brief Get the ID of the token pack
	 * @param In The token pack to retrieve the ID from
	 * @return The ID of the token pack
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Meta = (Category = "Token Pack"))
	static FString GetId(const FModioTokenPack& In)
	{
		return In.GetId();
	}

	/** 
	 * @docpublic
	 * @brief Get the Modio ID for this token pack
	 * @param In The token pack to retrieve the Modio ID from
	 * @return The Modio ID of the token pack
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Meta = (Category = "Token Pack"))
	static FModioTokenPackID GetModioId(const FModioTokenPack& In)
	{
		return In.GetModioId();
	}
};