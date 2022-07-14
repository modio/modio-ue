#include "UI/EventHandlers/IModioUIModInfoReceiver.h"

void IModioUIModInfoReceiver::ModInfoRequestHandler(FModioModID ModID, FModioErrorCode ec,
													TOptional<FModioModInfo> Info)
{
	bRoutedUIModInfoReceiver = false;
	NativeOnModInfoRequestCompleted(ModID, ec, Info);
	checkf(bRoutedUIModInfoReceiver,
		   TEXT("NativeOnModInfoRequestCompleted should call Super::NativeOnModInfoRequestCompleted "
				"to route events to blueprint"));
}

void IModioUIModInfoReceiver::ListAllModsRequestHandler(FString RequestIdentifier, FModioErrorCode ec,
														TOptional<FModioModInfoList> List)
{
	bRoutedUIModInfoReceiver = false;
	NativeOnListAllModsRequestCompleted(RequestIdentifier, ec, List);
	checkf(bRoutedUIModInfoReceiver,
		   TEXT("NativeOnListAllModsRequestCompleted should call Super::NativeOnListAllModsRequestCompleted "
				"to route events to blueprint"));
}

void IModioUIModInfoReceiver::NativeOnModInfoRequestCompleted(FModioModID ModID, FModioErrorCode ec,
															  TOptional<FModioModInfo> Info)
{
	bRoutedUIModInfoReceiver = true;
	Execute_OnModInfoRequestCompleted(Cast<UObject>(this), ModID, ec, FModioOptionalModInfo{Info});
}
void IModioUIModInfoReceiver::NativeOnListAllModsRequestCompleted(FString RequestIdentifier, FModioErrorCode ec,
																  TOptional<FModioModInfoList> List)
{
	bRoutedUIModInfoReceiver = true;
	Execute_OnListAllModsRequestCompleted(Cast<UObject>(this), RequestIdentifier, ec, FModioOptionalModInfoList(MoveTemp(List)));
}