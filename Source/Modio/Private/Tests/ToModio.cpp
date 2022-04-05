/* 
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *  
 *  This file is part of the mod.io UE4 Plugin.
 *  
 *  Distributed under the MIT License. (See accompanying file LICENSE or 
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *   
 */

#include "Internal/Convert/AuthParams.h"
#include "Libraries/ModioSDKLibrary.h"
#include "Misc/AutomationTest.h"
#include "ModioSDK.h"
#include "Types/ModioAuthenticationParams.h"
#include "Types/ModioModStats.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FModioConvertDataToNativeFormatTest,
								 "Modio.DataConversion.UnrealToNative.AuthProvider",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext |
									 EAutomationTestFlags::ProductFilter)
bool FModioConvertDataToNativeFormatTest::RunTest(const FString& Parameters)
{
	TestEqual("AuthProvider ToModio Xbox", ToModio(EModioAuthenticationProvider::XboxLive),
			  Modio::AuthenticationProvider::XboxLive);
	TestEqual("AuthProvider ToModio Steam", ToModio(EModioAuthenticationProvider::Steam),
			  Modio::AuthenticationProvider::Steam);
	TestEqual("AuthProvider ToModio GoG", ToModio(EModioAuthenticationProvider::GoG),
			  Modio::AuthenticationProvider::GoG);
	TestEqual("AuthProvider ToModio Itch", ToModio(EModioAuthenticationProvider::Itch),
			  Modio::AuthenticationProvider::Itch);
	TestEqual("AuthProvider ToModio Switch", ToModio(EModioAuthenticationProvider::Switch),
			  Modio::AuthenticationProvider::Switch);
	TestEqual("AuthProvider ToModio Discord", ToModio(EModioAuthenticationProvider::Discord),
			  Modio::AuthenticationProvider::Discord);
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS