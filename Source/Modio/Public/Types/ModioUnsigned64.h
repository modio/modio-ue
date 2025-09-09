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

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/KismetTextLibrary.h"

#include "ModioUnsigned64.generated.h"

/**
 * @docpublic
 * @brief Trivial Blueprint-compatible wrapper around an unsigned 64-bit integer
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioUnsigned64
{
	GENERATED_BODY()
	;

	/** 
	 * @docpublic
	 * @brief Underlying unsigned 64-bit integer value
	 */
	uint64 Underlying;

	/** 
	 * @docpublic
	 * @brief Default constructor initializing to zero
	 */
	FModioUnsigned64() : Underlying(0)
	{
	};

	/** 
	 * @docpublic
	 * @brief Copy constructor
	 */
	FModioUnsigned64(const FModioUnsigned64& Other) = default;

	/** 
	 * @docpublic
	 * @brief Move constructor
	 */
	FModioUnsigned64(FModioUnsigned64&& Other) = default;

	/** 
	 * @docpublic
	 * @brief Constructor that initializes with a specific unsigned 64-bit value
	 * @param Value The value to initialize with
	 */
	explicit FModioUnsigned64(uint64 Value) : Underlying(Value)
	{
	};

	/** 
	 * @docpublic
	 * @brief Conversion to uint64
	 */
	constexpr explicit operator uint64() const
	{
		return Underlying;
	}

	/** 
	 * @docpublic
	 * @brief Conversion to boolean
	 */
	constexpr explicit operator bool() const
	{
		return (bool)Underlying;
	}

	/** 
	 * @docpublic
	 * @brief Conversion to double
	 */
	constexpr explicit operator double() const
	{
		return double(Underlying);
	}

	/** 
	 * @docpublic
	 * @brief Conversion to float
	 */
	constexpr explicit operator float() const
	{
		return float(Underlying);
	}

	/**
	* @docpublic
	* @brief Conversion to int64
	*/
	constexpr explicit operator int64() const
	{
		return int64(Underlying);
	}

	/** 
	 * @docpublic
	 * @brief Copy assignment operator
	 */
	FORCEINLINE FModioUnsigned64& operator=(const FModioUnsigned64& Other)
	{
		if (this != &Other)
		{
			Underlying = Other.Underlying;
		}
		return *this;
	}

	/** 
	 * @docpublic
	 * @brief Less than comparison operator
	 */
	FORCEINLINE friend bool operator<(const FModioUnsigned64& LHS, const FModioUnsigned64& RHS)
	{
		return LHS.Underlying < RHS.Underlying;
	}

	/** 
	 * @docpublic
	 * @brief Greater than comparison operator
	 */
	FORCEINLINE friend bool operator>(const FModioUnsigned64& LHS, const FModioUnsigned64& RHS)
	{
		return LHS.Underlying > RHS.Underlying;
	}

	/** 
	 * @docpublic
	 * @brief Equality comparison operator
	 */
	FORCEINLINE friend bool operator==(const FModioUnsigned64& LHS, const FModioUnsigned64& RHS)
	{
		return LHS.Underlying == RHS.Underlying;
	}

	/** 
	 * @docpublic
	 * @brief Inequality comparison operator
	 */
	FORCEINLINE friend bool operator!=(const FModioUnsigned64& LHS, const FModioUnsigned64& RHS)
	{
		return !(LHS == RHS);
	}

	/** 
	 * @docpublic
	 * @brief Equality comparison operator with uint64
	 */
	FORCEINLINE friend bool operator==(const FModioUnsigned64& LHS, const uint64& RHS)
	{
		return LHS.Underlying == RHS;
	}

	/** 
	 * @docpublic
	 * @brief Division operator
	 */
	FORCEINLINE friend FModioUnsigned64 operator/(const FModioUnsigned64& LHS, const FModioUnsigned64& RHS)
	{
		return FModioUnsigned64(LHS.Underlying / RHS.Underlying);
	}

	/** 
	 * @docpublic
	 * @brief Addition operator
	 */
	FORCEINLINE friend FModioUnsigned64 operator+(const FModioUnsigned64& LHS, const FModioUnsigned64& RHS)
	{
		return FModioUnsigned64(LHS) += RHS;
	}

	/** 
	 * @docpublic
	 * @brief Subtraction operator
	 */
	FORCEINLINE friend FModioUnsigned64 operator-(const FModioUnsigned64& LHS, const FModioUnsigned64& RHS)
	{
		return FModioUnsigned64(LHS) -= RHS;
	}

	/** 
	 * @docpublic
	 * @brief Addition operator with uint64
	 */
	FORCEINLINE friend FModioUnsigned64 operator+(const FModioUnsigned64& LHS, const uint64 RHS)
	{
		return FModioUnsigned64(LHS.Underlying + RHS);
	}

	/** 
	 * @docpublic
	 * @brief Subtraction operator with uint64
	 */
	FORCEINLINE friend FModioUnsigned64 operator-(const FModioUnsigned64& LHS, const uint64 RHS)
	{
		return FModioUnsigned64(LHS.Underlying - RHS);
	}

	/** 
	 * @docpublic
	 * @brief Multiplication operator with uint64
	 */
	FORCEINLINE friend FModioUnsigned64 operator*(const FModioUnsigned64& LHS, const uint64 RHS)
	{
		return FModioUnsigned64(LHS.Underlying * RHS);
	}

	/** 
	 * @docpublic
	 * @brief Division operator with double
	 */
	FORCEINLINE friend double operator/(const FModioUnsigned64& LHS, double Divisor)
	{
		return double(LHS.Underlying) / Divisor;
	}

	/** 
	 * @docpublic
	 * @brief Addition assignment operator
	 */
	FORCEINLINE FModioUnsigned64& operator+=(const FModioUnsigned64& Other)
	{
		Underlying += Other.Underlying;
		return *this;
	}

	/** 
	 * @docpublic
	 * @brief Subtraction assignment operator
	 */
	FORCEINLINE FModioUnsigned64& operator-=(const FModioUnsigned64& Other)
	{
		Underlying -= Other.Underlying;
		return *this;
	}
};

/**
 * @docpublic
 * @brief Library class for operations on FModioUnsigned64
 */
UCLASS()
class MODIO_API UModioUnsigned64Library : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** 
	 * @docpublic
	 * @brief Compares two FModioUnsigned64 values for equality
	 */
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "mod.io|Unsigned64",
		meta = (CompactNodeTitle = "===", Keywords = "== equal",
			DisplayName = "ModioUnsigned64 == ModioUnsigned64"))
	static bool EqualTo(const FModioUnsigned64& LHS, const FModioUnsigned64& RHS)
	{
		return LHS == RHS;
	}

	/** 
	 * @docpublic
	 * @brief Compares two FModioUnsigned64 values for inequality
	 */
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "mod.io|Unsigned64",
		meta = (CompactNodeTitle = "!=", Keywords = "!= not equal",
			DisplayName = "ModioUnsigned64 != ModioUnsigned64"))
	static bool NotEqualTo(const FModioUnsigned64& LHS, const FModioUnsigned64& RHS)
	{
		return LHS != RHS;
	}

	/** 
	 * @docpublic
	 * @brief Checks if FModioUnsigned64 is greater than zero
	 */
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "mod.io|Unsigned64",
		meta = (CompactNodeTitle = "> 0", Keywords = "> greater than 0 zero",
			DisplayName = "ModioUnsigned64 > 0"))
	static bool GreaterThanZero(const FModioUnsigned64& In)
	{
		return In.Underlying > 0;
	}

	/** 
	 * @docpublic
	 * @brief Compares two FModioUnsigned64 values to see if the left-hand side is greater than the right-hand side
	 */
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "mod.io|Unsigned64",
		meta = (CompactNodeTitle = ">", Keywords = "> greater than",
			DisplayName = "ModioUnsigned64 > ModioUnsigned64"))
	static bool GreaterThan(const FModioUnsigned64& LHS, const FModioUnsigned64& RHS)
	{
		return LHS > RHS;
	}

	/** 
	 * @docpublic
	 * @brief Compares two FModioUnsigned64 values to see if the left-hand side is less than the right-hand side
	 */
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "mod.io|Unsigned64",
		meta = (CompactNodeTitle = "<", Keywords = "< less than",
			DisplayName = "ModioUnsigned64 < ModioUnsigned64"))
	static bool LessThan(const FModioUnsigned64& LHS, const FModioUnsigned64& RHS)
	{
		return LHS < RHS;
	}

	/** 
	 * @docpublic
	 * @brief Subtracts one FModioUnsigned64 from another
	 */
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "mod.io|Unsigned64",
		meta = (CompactNodeTitle = "-", Keywords = "- subtract minus",
			DisplayName = "ModioUnsigned64 - ModioUnsigned64"))
	static FModioUnsigned64 Subtract(const FModioUnsigned64& LHS, const FModioUnsigned64& RHS)
	{
		return LHS - RHS;
	}

	/** 
	 * @docpublic
	 * @brief Adds two FModioUnsigned64 values together
	 */
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "mod.io|Unsigned64",
		meta = (CompactNodeTitle = "+", Keywords = "+ add plus",
			DisplayName = "ModioUnsigned64 + ModioUnsigned64"))
	static FModioUnsigned64 Add(const FModioUnsigned64& LHS, const FModioUnsigned64& RHS)
	{
		return LHS + RHS;
	}

	/** 
	 * @docpublic
	 * @brief Divides two FModioUnsigned64 values (truncate result)
	 */
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "mod.io|Unsigned64",
		meta = (CompactNodeTitle = "/", Keywords = "/ divide",
			DisplayName = "ModioUnsigned64 / ModioUnsigned64 (truncate)"))
	static FModioUnsigned64 Divide(const FModioUnsigned64& LHS, const FModioUnsigned64& RHS)
	{
		return LHS / RHS;
	}

	/** 
	 * @docpublic
	 * @brief Divides two FModioUnsigned64 values and returns the result as a float
	 */
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "mod.io|Unsigned64",
		meta = (CompactNodeTitle = "/", Keywords = "/ divide", DisplayName = "ModioUnsigned64 / ModioUnsigned64"))
	static float DivideToFloat(const FModioUnsigned64& LHS, const FModioUnsigned64& RHS)
	{
		return (float)((double)LHS / (double)RHS);
	}

	/** 
	 * @docpublic
	 * @brief Divides an FModioUnsigned64 by a float and returns the result as a float
	 */
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "mod.io|Unsigned64",
		meta = (CompactNodeTitle = "/", Keywords = "/ divide", DisplayName = "ModioUnsigned64 / float"))
	static float DivideFloat(const FModioUnsigned64& LHS, const float RHS)
	{
		return (float)(LHS / (double)RHS);
	}

	/** 
	 * @docpublic
	 * @brief Calculates the percentage of one FModioUnsigned64 value in relation to another
	 */
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "mod.io|Unsigned64")
	static float Percentage_Unsigned64(const FModioUnsigned64& LHS, const FModioUnsigned64& RHS)
	{
		return DivideToFloat(LHS, RHS) * 100;
	}

	/** 
	 * @docpublic
	 * @brief Breaks a FModioUnsigned64 into two components: high and low 32-bits
	 */
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "mod.io|Unsigned64")
	static void BreakToComponents(const FModioUnsigned64& In, int32& High, int32& Low)
	{
		High = static_cast<int32>(In.Underlying >> 32);
		Low = static_cast<int32>(static_cast<uint32>(In.Underlying));
	}

	/** 
	 * @docpublic
	 * @brief Creates an FModioUnsigned64 from two 32-bit components
	 */
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "mod.io|Unsigned64")
	static FModioUnsigned64 MakeFromComponents(const int32& High, const int32& Low)
	{
		uint64 RawValue = static_cast<uint32>(High);
		RawValue = RawValue << 32;
		RawValue |= static_cast<uint32>(Low);
		return FModioUnsigned64(RawValue);
	}

	/** 
	 * @docpublic
	 * @brief Converts FModioUnsigned64 to a float
	 */
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "mod.io|Unsigned64",
		meta = (DisplayName = "TruncateModioUnsigned64ToFloat", BlueprintAutocast))
	static float Conv_FModioUnsigned64ToFloat(const FModioUnsigned64& In)
	{
		return float(In.Underlying);
	}

	/** 
	 * @docpublic
	 * @brief Converts FModioUnsigned64 to text
	 */
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "mod.io|Unsigned64",
		meta = (DisplayName = "To Text (ModioUnsigned64)", BlueprintAutocast))
	static FText Conv_FModioUnsigned64ToText(const FModioUnsigned64& In)
	{
		return UKismetTextLibrary::Conv_Int64ToText(int64_t(In.Underlying));
	}

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "mod.io|Unsigned64",
		meta = (DisplayName = "To Int64 (ModioUnsigned64)", BlueprintAutocast))
	static int64 Conv_FModioUnsigned64ToInt64(const FModioUnsigned64& In)
	{
		return int64(In);
	}
};
