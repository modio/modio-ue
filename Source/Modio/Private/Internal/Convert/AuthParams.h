/* 
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *  
 *  This file is part of the mod.io UE Plugin.
 *  
 *  Distributed under the MIT License. (See accompanying file LICENSE or 
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *   
 */

#pragma once

#include "ModioSDK.h"
#include "GenericPlatform/GenericPlatformHttp.h"
#include "Internal/ModioConvert.h"
#include "Internal/ModioPrivateDefines.h"
#include "Misc/Optional.h"
#include "Types/ModioAuthenticationParams.h"


MODIO_BEGIN_CONVERT_SWITCHES
FORCEINLINE Modio::AuthenticationProvider ToModio(EModioAuthenticationProvider Provider)
{
	switch (Provider)
	{
		case EModioAuthenticationProvider::XboxLive:
			return Modio::AuthenticationProvider::XboxLive;
		case EModioAuthenticationProvider::Steam:
			return Modio::AuthenticationProvider::Steam;
		case EModioAuthenticationProvider::GoG:
			return Modio::AuthenticationProvider::GoG;
		case EModioAuthenticationProvider::Itch:
			return Modio::AuthenticationProvider::Itch;
		case EModioAuthenticationProvider::Switch:
			return Modio::AuthenticationProvider::Switch;
		case EModioAuthenticationProvider::Discord:
			return Modio::AuthenticationProvider::Discord;
		case EModioAuthenticationProvider::PSN:
			return Modio::AuthenticationProvider::PSN;
		case EModioAuthenticationProvider::Epic:
			return Modio::AuthenticationProvider::Epic;
		case EModioAuthenticationProvider::Oculus:
			return Modio::AuthenticationProvider::Oculus;
		case EModioAuthenticationProvider::OpenID:
			return Modio::AuthenticationProvider::OpenID;
		case EModioAuthenticationProvider::GoogleIDToken:
			return Modio::AuthenticationProvider::GoogleIDToken;
		case EModioAuthenticationProvider::GoogleServerSideToken:
			return Modio::AuthenticationProvider::GoogleServerSideToken;
	}

	return Modio::AuthenticationProvider::Steam;
}

MODIO_END_CONVERT_SWITCHES


FORCEINLINE Modio::AuthenticationParams ToModio(const FModioAuthenticationParams& UnrealParams)
{
	Modio::AuthenticationParams Params;
	Params.AuthToken = ToModio(FGenericPlatformHttp::UrlEncode(UnrealParams.AuthToken));
	Params.UserEmail = UnrealParams.UserEmail.TrimStartAndEnd().IsEmpty()
		                   ? Modio::Optional<std::string>(ToModio(UnrealParams.UserEmail))
		                   : Modio::Optional<std::string>();
	Params.bUserHasAcceptedTerms = UnrealParams.bUserHasAcceptedTerms;
	Params.ExtendedParameters = ToModio(UnrealParams.ExtendedParameters);

	return Params;
}
