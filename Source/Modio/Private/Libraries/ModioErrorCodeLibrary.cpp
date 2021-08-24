#include "Libraries/ModioErrorCodeLibrary.h"

bool UModioErrorCodeLibrary::IsError(const FModioErrorCode& Error)
{
	return Error;
}

FString UModioErrorCodeLibrary::GetMessage(const FModioErrorCode& Error)
{
	return Error.GetMessage();
}

int32 UModioErrorCodeLibrary::GetValue(const FModioErrorCode& Error)
{
	return Error.GetValue();
}
