// Fill out your copyright notice in the Description page of Project Settings.


#include "Libraries/ModioPlatformHelpersLibrary.h"

#include "Misc/ConfigCacheIni.h"
#include "Types/ModioAuthenticationParams.h"
#include "Types/ModioCommonTypes.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioPlatformHelpersLibrary)

EModioPlatformName UModioPlatformHelpersLibrary::GetCurrentPlatform()
{
	#if PLATFORM_WINDOWS
		return EModioPlatformName::Windows;
	#endif

	#if PLATFORM_MAC
		return EModioPlatformName::Mac;
	#endif

	#if PLATFORM_IOS
		return EModioPlatformName::iOS;
	#endif

	#if PLATFORM_LINUX
		return EModioPlatformName::Linux;
	#endif

	#if defined(PLATFORM_PS4) && PLATFORM_PS4
		return EModioPlatformName::PS4;
	#endif

	#if defined(PLATFORM_PS5) && PLATFORM_PS5
		return EModioPlatformName::PS5;
	#endif

	#if defined(PLATFORM_XBOXONE) && PLATFORM_XBOXONE
		return EModioPlatformName::XBoxOne;
	#endif

	#if defined(PLATFORM_XSX) && PLATFORM_XSX
		return EModioPlatformName::XSX;
	#endif

	#if PLATFORM_SWITCH
		return EModioPlatformName::Switch;
	#endif

	#if PLATFORM_ANDROID
		return EModioPlatformName::Android;
	#endif
}

EModioPortal UModioPlatformHelpersLibrary::GetDefaultPortalForCurrentPlatform()
{
	switch (GetCurrentPlatform())
	{
		case EModioPlatformName::Windows:
		case EModioPlatformName::Mac:
		case EModioPlatformName::Linux:
			// We should be handling other Portals for desktop platforms here.
			return EModioPortal::Steam;
		case EModioPlatformName::PS4:
		case EModioPlatformName::PS5:
			return EModioPortal::PSN;
		case EModioPlatformName::XBoxOne:
		case EModioPlatformName::XSX:
			return EModioPortal::XboxLive;
		case EModioPlatformName::Switch:
			return EModioPortal::Nintendo;
		case EModioPlatformName::Android:
			return EModioPortal::Google;
		case EModioPlatformName::Unknown:
		case EModioPlatformName::iOS:
			return EModioPortal::None;
		default:
			return EModioPortal::None;
	}
}

EModioAuthenticationProvider UModioPlatformHelpersLibrary::GetDefaultAuthProviderForCurrentPlatform()
{
	switch (GetCurrentPlatform())
	{
		case EModioPlatformName::Windows:
		case EModioPlatformName::Mac:
		case EModioPlatformName::Linux:
		{
			// We should be handling other Auth Providers for desktop platforms here.
			FString InterfaceString;
			GConfig->GetString(TEXT("OnlineSubsystem"), TEXT("NativePlatformService"), InterfaceString, GEngineIni);
			if (InterfaceString == TEXT("GOG"))
				return EModioAuthenticationProvider::GoG;
			else if (InterfaceString == TEXT("Steam"))
				return EModioAuthenticationProvider::Steam;
			else if (InterfaceString == TEXT("EOS"))
				return EModioAuthenticationProvider::Epic;
			else
				return EModioAuthenticationProvider::Steam;
		}
		case EModioPlatformName::PS4:
		case EModioPlatformName::PS5:
			return EModioAuthenticationProvider::PSN;
		case EModioPlatformName::XBoxOne:
		case EModioPlatformName::XSX:
			return EModioAuthenticationProvider::XboxLive;
		case EModioPlatformName::Switch:
			return EModioAuthenticationProvider::Switch;
		case EModioPlatformName::Android:
			// By default, we use the ServerSideToken for Google Auth as opposed to an ID Token
			return EModioAuthenticationProvider::GoogleServerSideToken;

		case EModioPlatformName::iOS:
		case EModioPlatformName::Unknown:
		default:;
	}

	return {};
}

