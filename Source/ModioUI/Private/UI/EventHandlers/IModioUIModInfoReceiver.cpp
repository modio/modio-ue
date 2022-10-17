/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

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
	Execute_OnModInfoRequestCompleted(Cast<UObject>(this), ModID, ec, FModioOptionalModInfo {Info});
}
void IModioUIModInfoReceiver::NativeOnListAllModsRequestCompleted(FString RequestIdentifier, FModioErrorCode ec,
																  TOptional<FModioModInfoList> List)
{
	bRoutedUIModInfoReceiver = true;
	Execute_OnListAllModsRequestCompleted(Cast<UObject>(this), RequestIdentifier, ec,
										  FModioOptionalModInfoList(MoveTemp(List)));
}