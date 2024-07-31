/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "Types/ModioErrorCode.h"
#include "ModioSDK.h"

FModioErrorCode::FModioErrorCode(Modio::ErrorCode ec)
{
	InternalError = MakeUnique<Modio::ErrorCode>(ec);
}

FModioErrorCode::FModioErrorCode()
{
	InternalError = MakeUnique<Modio::ErrorCode>();
}

FModioErrorCode::FModioErrorCode(const FModioErrorCode& Other)
{
	InternalError = MakeUnique<Modio::ErrorCode>(Other.GetRawErrorCode());
}

FModioErrorCode::FModioErrorCode(int32 Value, int32 CategoryID)
{
	InternalError = MakeUnique<Modio::ErrorCode>(Value, Modio::Detail::GetModioErrorCategoryByID(CategoryID));
}

FModioErrorCode& FModioErrorCode::operator=(const FModioErrorCode& ec)
{
	InternalError = MakeUnique<Modio::ErrorCode>(ec.GetRawErrorCode());
	return *this;
}

FModioErrorCode::~FModioErrorCode()
{
	InternalError.Reset();
}

FModioErrorCode::operator bool() const
{
	return InternalError->value() != 0;
}

int FModioErrorCode::GetValue() const
{
	return InternalError->value();
}

FString FModioErrorCode::GetCategoryString() const
{
	return FString(InternalError->category().name());
}

Modio::ErrorCode FModioErrorCode::GetRawErrorCode() const
{
	return *InternalError;
}

FString FModioErrorCode::GetErrorMessage() const
{
	if (InternalError.IsValid())
	{
		return FString(UTF8_TO_TCHAR(InternalError->message().c_str()));
	}
	else
	{
		return FString();
	}
}

FModioErrorCode FModioErrorCode::SystemError()
{
	static FModioErrorCode ec = FModioErrorCode(Modio::make_error_code(Modio::SystemError::UnknownSystemError));
	return ec;
}

int32 FModioErrorCode::GetCategoryID()
{
	if (InternalError)
	{
		return Modio::Detail::ModioErrorCategoryID(InternalError->category());
	}
	else
	{
		return -1;
	}
}
