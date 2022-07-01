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

#include "Kismet/BlueprintFunctionLibrary.h"

#include "ModioUnsigned64.generated.h"

/// @brief Trivial Blueprint-compatible wrapper around an unsigned 64-bit integer
USTRUCT(BlueprintType)
struct MODIO_API FModioUnsigned64
{
	GENERATED_BODY();

	uint64 Underlying;

	FModioUnsigned64() : Underlying(0) {};
	FModioUnsigned64(const FModioUnsigned64& Other) = default;
	FModioUnsigned64(FModioUnsigned64&& Other) = default;

	explicit FModioUnsigned64(uint64 Value) : Underlying(Value) {};

	constexpr explicit operator uint64() const
	{
		return Underlying;
	}

	constexpr explicit operator bool() const
	{
		return (bool) Underlying;
	}

	constexpr explicit operator double() const
	{
		return double(Underlying);
	}
	constexpr explicit operator float() const
	{
		return float(Underlying);
	}
	FORCEINLINE FModioUnsigned64& operator=(const FModioUnsigned64& Other)
	{
		if (this != &Other)
		{
			Underlying = Other.Underlying;
		}
		return *this;
	}

	FORCEINLINE friend bool operator<(const FModioUnsigned64& LHS, const FModioUnsigned64& RHS)
	{
		return LHS.Underlying < RHS.Underlying;
	}
	FORCEINLINE friend bool operator>(const FModioUnsigned64& LHS, const FModioUnsigned64& RHS)
	{
		return LHS.Underlying > RHS.Underlying;
	}
	FORCEINLINE friend bool operator==(const FModioUnsigned64& LHS, const FModioUnsigned64& RHS)
	{
		return LHS.Underlying == RHS.Underlying;
	}
	FORCEINLINE friend bool operator!=(const FModioUnsigned64& LHS, const FModioUnsigned64& RHS)
	{
		return !(LHS == RHS);
	}
	FORCEINLINE friend bool operator==(const FModioUnsigned64& LHS, const uint64& RHS)
	{
		return LHS.Underlying == RHS;
	}

	FORCEINLINE friend FModioUnsigned64 operator/(const FModioUnsigned64& LHS, const FModioUnsigned64& RHS)
	{
		return FModioUnsigned64(LHS.Underlying / RHS.Underlying);
	}

	FORCEINLINE friend FModioUnsigned64 operator+(const FModioUnsigned64& LHS, const FModioUnsigned64& RHS)
	{
		return FModioUnsigned64(LHS) += RHS;
	}

	FORCEINLINE friend FModioUnsigned64 operator-(const FModioUnsigned64& LHS, const FModioUnsigned64& RHS)
	{
		return FModioUnsigned64(LHS) -= RHS;
	}
	FORCEINLINE friend FModioUnsigned64 operator+(const FModioUnsigned64& LHS, const uint64 RHS)
	{
		return FModioUnsigned64(LHS.Underlying + RHS);
	}
	FORCEINLINE friend FModioUnsigned64 operator-(const FModioUnsigned64& LHS, const uint64 RHS)
	{
		return FModioUnsigned64(LHS.Underlying - RHS);
	}
	FORCEINLINE friend FModioUnsigned64 operator*(const FModioUnsigned64& LHS, const uint64 RHS)
	{
		return FModioUnsigned64(LHS.Underlying * RHS);
	}

	FORCEINLINE friend double operator/(const FModioUnsigned64& LHS, double Divisor)
	{
		return LHS.Underlying / Divisor;
	}

	FORCEINLINE FModioUnsigned64& operator+=(const FModioUnsigned64& Other)
	{
		Underlying += Other.Underlying;
		return *this;
	}
	FORCEINLINE FModioUnsigned64& operator-=(const FModioUnsigned64& Other)
	{
		Underlying -= Other.Underlying;
		return *this;
	}
};

UCLASS()
class MODIO_API UModioUnsigned64Library : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "mod.io|Unsigned64",
			  meta = (CompactNodeTitle = "===", Keywords = "== equal",
					  DisplayName = "ModioUnsigned64 == ModioUnsigned64"))
	static bool EqualTo(const FModioUnsigned64& LHS, const FModioUnsigned64& RHS)
	{
		return LHS == RHS;
	}

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "mod.io|Unsigned64",
			  meta = (CompactNodeTitle = "!=", Keywords = "!= not equal",
					  DisplayName = "ModioUnsigned64 != ModioUnsigned64"))
	static bool NotEqualTo(const FModioUnsigned64& LHS, const FModioUnsigned64& RHS)
	{
		return LHS != RHS;
	}

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "mod.io|Unsigned64",
			  meta = (CompactNodeTitle = ">", Keywords = "> greater than",
					  DisplayName = "ModioUnsigned64 > ModioUnsigned64"))
	static bool GreaterThan(const FModioUnsigned64& LHS, const FModioUnsigned64& RHS)
	{
		return LHS > RHS;
	}

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "mod.io|Unsigned64",
			  meta = (CompactNodeTitle = "<", Keywords = "< less than",
					  DisplayName = "ModioUnsigned64 < ModioUnsigned64"))
	static bool LessThan(const FModioUnsigned64& LHS, const FModioUnsigned64& RHS)
	{
		return LHS < RHS;
	}

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "mod.io|Unsigned64",
			  meta = (CompactNodeTitle = "-", Keywords = "- subtract minus",
					  DisplayName = "ModioUnsigned64 - ModioUnsigned64"))
	static FModioUnsigned64 Subtract(const FModioUnsigned64& LHS, const FModioUnsigned64& RHS)
	{
		return LHS - RHS;
	}

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "mod.io|Unsigned64",
			  meta = (CompactNodeTitle = "+", Keywords = "+ add plus",
					  DisplayName = "ModioUnsigned64 + ModioUnsigned64"))
	static FModioUnsigned64 Add(const FModioUnsigned64& LHS, const FModioUnsigned64& RHS)
	{
		return LHS + RHS;
	}

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "mod.io|Unsigned64",
			  meta = (CompactNodeTitle = "/", Keywords = "/ divide",
					  DisplayName = "ModioUnsigned64 / ModioUnsigned64 (truncate)"))
	static FModioUnsigned64 Divide(const FModioUnsigned64& LHS, const FModioUnsigned64& RHS)
	{
		return LHS / RHS;
	}

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "mod.io|Unsigned64",
			  meta = (CompactNodeTitle = "/", Keywords = "/ divide", DisplayName = "ModioUnsigned64 / ModioUnsigned64"))
	static float DivideToFloat(const FModioUnsigned64& LHS, const FModioUnsigned64& RHS)
	{
		return (float) ((double) LHS / (double) RHS);
	}

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "mod.io|Unsigned64",
			  meta = (CompactNodeTitle = "/", Keywords = "/ divide", DisplayName = "ModioUnsigned64 / float"))
	static float DivideFloat(const FModioUnsigned64& LHS, const float RHS)
	{
		return (float) (LHS / (double) RHS);
	}

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "mod.io|Unsigned64")
	static float Percentage_Unsigned64(const FModioUnsigned64& LHS, const FModioUnsigned64& RHS)
	{
		return DivideToFloat(LHS, RHS) * 100;
	}

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "mod.io|Unsigned64")
	static void BreakToComponents(const FModioUnsigned64& In, int32& High, int32& Low)
	{
		High = static_cast<int32>(In.Underlying >> 32);
		Low = static_cast<int32>(static_cast<uint32>(In.Underlying));
	}
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "mod.io|Unsigned64")
	static FModioUnsigned64 MakeFromComponents(const int32& High, const int32& Low)
	{
		uint64 RawValue = static_cast<uint32>(High);
		RawValue = RawValue << 32;
		RawValue |= static_cast<uint32>(Low);
		return FModioUnsigned64(RawValue);
	}

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "mod.io|Unsigned64",
			  meta = (DisplayName = "TruncateModioUnsigned64ToFloat", BlueprintAutocast))
	static float Conv_FModioUnsigned64ToFloat(const FModioUnsigned64& In)
	{
		return float(In.Underlying);
	}
};