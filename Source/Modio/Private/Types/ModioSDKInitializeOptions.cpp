#include "Types/ModioInitializeOptions.h"
#include "Modio.h"
// For GetUserSidString
#include <processthreadsapi.h>
#include <sddl.h>

static std::string GetUserSidString()
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


FModioInitializeOptions::operator Modio::InitializeOptions() const
{
	Modio::InitializeOptions Options;
	Options.GameID = GameID;
	Options.APIKey = APIKey;
	Options.GameEnvironment = ToModio(GameEnvironment);
	Options.User = GetUserSidString();
	Options.PortalInUse = ToModio(PortalInUse);
	return Options;
}
