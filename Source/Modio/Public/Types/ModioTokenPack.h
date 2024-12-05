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
 * @brief A wrapper type around the Unreal Engine type FOnlineStoreOffer, representing a platform-agnostic store offer/product, e.g a Premium Currency pack.
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioTokenPack
{
	GENERATED_BODY()

private:

	FOnlineStoreOffer Underlying;

public:

	FModioTokenPack() = default;

	FModioTokenPack(const FOnlineStoreOffer& InOffer)
	{
		Underlying = InOffer;
	}

	FText GetTitle() const
	{
		return Underlying.Title;
	}

	FText GetDescription() const
	{
		return Underlying.Description;
	}

	FText GetLongDescription() const
	{
		return Underlying.LongDescription;
	}

	FText GetRegularPrice() const
	{
		return Underlying.GetDisplayRegularPrice();
	}

	FText GetDisplayPrice() const
	{
		return Underlying.GetDisplayPrice();
	}

	int64 GetNumericPrice() const
	{
		return Underlying.NumericPrice;
	}

	FString GetCurrencyCode() const
	{
		return Underlying.CurrencyCode;
	}

	FDateTime GetReleaseDate() const
	{
		return Underlying.ReleaseDate;
	}

	FDateTime GetExpirationDate() const
	{
		return Underlying.ExpirationDate;
	}

	bool IsPurchasable() const
	{
		return Underlying.IsPurchaseable();
	}

	TMap<FString, FString> GetFields() const
	{
		return Underlying.DynamicFields;
	}

	FString GetId() const
	{
		return Underlying.OfferId;
	}

	FModioTokenPackID GetModioId() const
	{
		return FModioTokenPackID(Underlying.OfferId);
	}
	
};

USTRUCT(BlueprintType)
struct MODIO_API FModioOptionalTokenPack
{
	GENERATED_BODY()

	TOptional<FModioTokenPack> Internal;
};

UCLASS()
class UModioTokenPackLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintPure, Meta = (Category = "Token Pack"))
	static FText GetTitle(const FModioTokenPack& In)
	{
		return In.GetTitle();
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Meta = (Category = "Token Pack"))
	static FText GetDescription(const FModioTokenPack& In)
	{
		return In.GetDescription();
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Meta = (Category = "Token Pack"))
	static FText GetLongDescription(const FModioTokenPack& In)
	{
		return In.GetLongDescription();
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Meta = (Category = "Token Pack"))
	static FText GetRegularPrice(const FModioTokenPack& In)
	{
		return In.GetRegularPrice();
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Meta = (Category = "Token Pack"))
	static FText GetDisplayPrice(const FModioTokenPack& In)
	{
		return In.GetDisplayPrice();
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Meta = (Category = "Token Pack"))
	static int64 GetNumericPrice(const FModioTokenPack& In)
	{
		return In.GetNumericPrice();
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Meta = (Category = "Token Pack"))
	static bool IsPurchasable(const FModioTokenPack& In)
	{
		return In.IsPurchasable();
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Meta = (Category = "Token Pack"))
	static TMap<FString, FString> GetFields(const FModioTokenPack& In)
	{
		return In.GetFields();
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Meta = (Category = "Token Pack"))
	static FString GetId(const FModioTokenPack& In)
	{
		return In.GetId();
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Meta = (Category = "Token Pack"))
	static FModioTokenPackID GetModioId(const FModioTokenPack& In)
	{
		return In.GetModioId();
	}

};