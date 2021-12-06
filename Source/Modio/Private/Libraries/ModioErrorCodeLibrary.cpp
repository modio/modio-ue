/* 
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *  
 *  This file is part of the mod.io UE4 Plugin.
 *  
 *  Distributed under the MIT License. (See accompanying file LICENSE or 
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *   
 */

#include "Libraries/ModioErrorCodeLibrary.h"

bool UModioErrorCodeLibrary::IsError(const FModioErrorCode& Error)
{
	return Error;
}

FString UModioErrorCodeLibrary::GetMessage(const FModioErrorCode& Error)
{
	return Error.GetErrorMessage();
}

int32 UModioErrorCodeLibrary::GetValue(const FModioErrorCode& Error)
{
	return Error.GetValue();
}
