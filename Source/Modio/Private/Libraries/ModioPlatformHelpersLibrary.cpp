// Fill out your copyright notice in the Description page of Project Settings.


#include "Libraries/ModioPlatformHelpersLibrary.h"

#include "Types/ModioAuthenticationParams.h"
#include "Types/ModioCommonTypes.h"

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

	#if PLATFORM_PS4
		return EModioPlatformName::PS4;
	#endif

	#if PLATFORM_PS5
		return EModioPlatformName::PS5;
	#endif

	#if PLATFORM_XBOXONE
		return EModioPlatformName::XBoxOne;
	#endif

	#if PLATFORM_XSX
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
			// We should be handling other Auth Providers for desktop platforms here.
			return EModioAuthenticationProvider::Steam;
		case EModioPlatformName::PS4:
		case EModioPlatformName::PS5:
			return EModioAuthenticationProvider::PSN;
		case EModioPlatformName::XBoxOne:
		case EModioPlatformName::XSX:
			return EModioAuthenticationProvider::XboxLive;
		case EModioPlatformName::Switch:
			return EModioAuthenticationProvider::Switch;
		default:;
	}

	return {};
}

