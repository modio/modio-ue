#include "Types/ModioErrorCode.h"
#include <system_error>

FModioErrorCode::FModioErrorCode(Modio::ErrorCode ec) : Error(ec) {}

FString FModioErrorCode::GetMessage() const
{
	return FString(UTF8_TO_TCHAR(Error.message().c_str()));
}
