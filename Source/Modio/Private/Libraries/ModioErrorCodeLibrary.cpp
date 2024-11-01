/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "Libraries/ModioErrorCodeLibrary.h"

bool UModioErrorCodeLibrary::IsError(const FModioErrorCode& Error)
{
	return Error;
}

bool UModioErrorCodeLibrary::IsErrorAsExec(const FModioErrorCode& Error)
{
	return IsError(Error);
}

FString UModioErrorCodeLibrary::GetMessage(const FModioErrorCode& Error)
{
	return Error.GetErrorMessage();
}


FModioErrorCode UModioErrorCodeLibrary::ReconstructError(int32 Value, int32 Category) 
{
	return FModioErrorCode(Value, Category);
}

int32 UModioErrorCodeLibrary::GetValue(const FModioErrorCode& Error)
{
	return Error.GetValue();
}
