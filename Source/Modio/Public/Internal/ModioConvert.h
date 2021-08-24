#pragma once

#include <chrono>
#include <string>
#include <vector>

#include "Containers/StringConv.h"
#include "Containers/UnrealString.h"
#include "Misc/DateTime.h"
#include "ModioSDK.h"

FORCEINLINE std::string ToSTD(const FString& String);
FORCEINLINE std::vector<std::string> ToSTD(const TArray<FString>& StringArray);
FORCEINLINE std::chrono::system_clock::time_point ToSTD(FDateTime Time);

// @todo: Make a ToModio that handles TOptional through templates

FORCEINLINE int64 ToUnreal(std::int64_t Value);
FORCEINLINE uint8 ToUnreal(std::uint8_t Value);
FORCEINLINE double ToUnreal(double Value);
FORCEINLINE bool ToUnreal(bool Value);
FORCEINLINE uint64 ToUnreal(std::size_t Value);
FORCEINLINE FString ToUnreal(const std::string& String);
FORCEINLINE FString ToUnreal(const Modio::filesystem::path& Path);
FORCEINLINE FDateTime ToUnrealDateTime(std::int64_t UnixTimestamp);

template<typename DestValueType, typename SourceValueType, typename... OtherParams>
FORCEINLINE TArray<DestValueType> ToUnreal(std::vector<SourceValueType, OtherParams...>&& OriginalArray);

template<typename DestValueType, typename SourceValueType, typename... OtherParams>
FORCEINLINE TArray<DestValueType> ToUnreal(const std::vector<SourceValueType, OtherParams...>& OriginalArray);

#pragma region Implementation

// @todo: Rename this to ToModio as it doesn't matter that it's the part of STD of modio
std::string ToSTD(const FString& String)
{
	return std::string(TCHAR_TO_UTF8(*String));
}

// @todo: Rename this to ToModio as it doesn't matter that it's the part of STD of modio
std::vector<std::string> ToSTD(const TArray<FString>& StringArray)
{
	std::vector<std::string> Result;
	Result.reserve(StringArray.Num());
	for (const FString& It : StringArray)
	{
		Result.emplace_back(TCHAR_TO_UTF8(*It));
	}
	return Result;
}

// @todo: Rename this to ToModio as it doesn't matter that it's the part of STD of modio
std::chrono::system_clock::time_point ToSTD(FDateTime Time)
{
	// @todonow: Verify that this becomes correct by printf debugging
	return std::chrono::system_clock::time_point(std::chrono::system_clock::duration(Time.ToUnixTimestamp()));
}

int64 ToUnreal(std::int64_t Value)
{
	return Value;
}

bool ToUnreal(bool Value)
{
	return Value;
}

uint8 ToUnreal(std::uint8_t Value)
{
	return Value;
}

double ToUnreal(double Value)
{
	return Value;
}

uint64 ToUnreal(std::size_t Value)
{
	static_assert(sizeof(std::size_t) == sizeof(uint64), "size_t is not 64-bits wide. Are you in x64 configuration?");
	return Value;
}

FString ToUnreal(const std::string& String)
{
	return UTF8_TO_TCHAR(String.c_str());
}

FString ToUnreal(const Modio::filesystem::path& Path)
{
	return UTF8_TO_TCHAR(Path.generic_u8string().c_str());
}

FDateTime FORCEINLINE ToUnrealDateTime(std::int64_t UnixTimestamp)
{
	return FDateTime::FromUnixTimestamp(UnixTimestamp);
}

template<typename DestValueType, typename SourceValueType, typename... OtherParams>
TArray<DestValueType> ToUnreal(std::vector<SourceValueType, OtherParams...>&& OriginalArray)
{
	TArray<DestValueType> Result;

	Result.Reserve(OriginalArray.size());
	for (auto& It : OriginalArray)
	{
		Result.Emplace(ToUnreal(MoveTemp(It)));
	}

	return Result;
}

template<typename DestValueType, typename SourceValueType, typename... OtherParams>
TArray<DestValueType> ToUnreal(const std::vector<SourceValueType, OtherParams...>& OriginalArray)
{
	TArray<DestValueType> Result;

	Result.Reserve(OriginalArray.size());
	for (const auto& It : OriginalArray)
	{
		Result.Emplace(ToUnreal(It));
	}

	return Result;
}

#pragma endregion
