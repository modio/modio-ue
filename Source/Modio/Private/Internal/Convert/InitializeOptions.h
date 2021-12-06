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
#include "Internal/ModioConvert.h"
#include "Internal/ModioPrivateDefines.h"
#include "ModioSDK.h"
#include "Types/ModioInitializeOptions.h"
#include "Modio.h"

#if PLATFORM_WINDOWS
// For GetUserSidString
#include <processthreadsapi.h>
#include <sddl.h>

static FORCEINLINE std::string GetUserSidString()
{
	LPSTR SidString = nullptr;

	HANDLE ProcessHandle = ::OpenProcess(PROCESS_QUERY_INFORMATION, false, GetCurrentProcessId());
	HANDLE TokenHandle;
	if (::OpenProcessToken(ProcessHandle, TOKEN_QUERY, &TokenHandle))
	{
		DWORD UserTokenSize;

		::GetTokenInformation(TokenHandle, TokenUser, NULL, 0, &UserTokenSize);

		if (::GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			PTOKEN_USER UserToken = reinterpret_cast<PTOKEN_USER>(new BYTE[UserTokenSize]);

			if (UserToken != NULL)
			{
				if (::GetTokenInformation(TokenHandle, TokenUser, UserToken, UserTokenSize, &UserTokenSize))
				{
					ConvertSidToStringSidA(UserToken->User.Sid, &SidString);
				}
			}
		}
		::CloseHandle(TokenHandle);
	}
	::CloseHandle(ProcessHandle);

	if (SidString == nullptr)
	{
		UE_LOG(LogModio, Warning, TEXT("Didn't manage to get the local user SID"));
		return "unknown-sid";
	}

	std::string SIDStringCopy(SidString);
	LocalFree(SidString);

	return SIDStringCopy;
}

#else

#endif

FORCEINLINE Modio::InitializeOptions ToModio(const FModioInitializeOptions& In )
{
    Modio::InitializeOptions Options;
    Options.GameID = ToModio(In.GameId);
    Options.APIKey = ToModio(In.ApiKey);
    Options.GameEnvironment = ToModio(In.GameEnvironment);
	#if PLATFORM_WINDOWS

    Options.User = In.LocalSessionIdentifier.IsSet() ? ToModio(In.LocalSessionIdentifier.GetValue()) : GetUserSidString();
	
	#else
	checkf(In.LocalSessionIdentifier.IsSet(), TEXT("Please set LocalSessionIdentifier on your InitializeOptions before passing them to InitializeAsync"))
	Options.User = ToModio(In.LocalSessionIdentifier.GetValue());
	#endif
    Options.PortalInUse = ToModio(In.PortalInUse);
    return Options;
}