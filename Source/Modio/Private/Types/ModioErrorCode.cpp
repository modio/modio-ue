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
	Error = MakeUnique<Modio::ErrorCode>(ec);
}

 FModioErrorCode::FModioErrorCode() 
 {
	Error = MakeUnique<Modio::ErrorCode>();
 }

 FModioErrorCode::FModioErrorCode(const FModioErrorCode& Other) 
 {
	Error = MakeUnique<Modio::ErrorCode>(Other.GetRawErrorCode());
 }

 FModioErrorCode& FModioErrorCode::operator=(const FModioErrorCode& ec) 
{
	Error = MakeUnique<Modio::ErrorCode>(ec.GetRawErrorCode());
	return *this;
}

 FModioErrorCode::~FModioErrorCode() 
 {
	Error.Reset();
 }

FModioErrorCode::operator bool() const
{
	return Error->value() != 0;
}

int FModioErrorCode::GetValue() const
{
	return Error->value();
}

Modio::ErrorCode FModioErrorCode::GetRawErrorCode() const
{
	return *Error;
}

FString FModioErrorCode::GetMessage() const
{
	if (*Error)
	{
		return FString(UTF8_TO_TCHAR(Error->message().c_str()));
	}
	else
	{
		return FString();
	}
}

