/* 
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *  
 *  This file is part of the mod.io SDK.
 *  
 *  Distributed under the MIT License. (See accompanying file LICENSE or 
 *   view online at <https://github.com/modio/modio-sdk/blob/main/LICENSE>)
 *  
 */

#pragma once
// Implementation header - do not include directly

#ifdef MODIO_SEPARATE_COMPILATION
	#include "modio/ModioSDK.h"
#endif

#include "modio/core/ModioCoreTypes.h"
#include "modio/detail/ops/AuthenticateUserByEmailOp.h"
#include "modio/detail/ops/RequestEmailAuthCodeOp.h"
#include "modio/detail/ops/auth/AuthenticateUserByDiscord.h"
#include "modio/detail/ops/auth/AuthenticateUserByGog.h"
#include "modio/detail/ops/auth/AuthenticateUserByItch.h"
#include "modio/detail/ops/auth/AuthenticateUserBySteam.h"
#include "modio/detail/ops/auth/AuthenticateUserBySwitchID.h"
#include "modio/detail/ops/auth/AuthenticateUserByXBoxLive.h"
#include "modio/detail/ops/auth/ModioGetTermsOfUseOp.h"
#include "modio/detail/ops/user/GetUserMediaOp.h"
#include "modio/impl/SDKPreconditionChecks.h"
#include "modio/userdata/ModioUserDataService.h"

namespace Modio
{
	void RequestEmailAuthCodeAsync(Modio::EmailAddress EmailAddress, std::function<void(Modio::ErrorCode)> Callback)
	{
		return asio::async_compose<std::function<void(Modio::ErrorCode)>, void(Modio::ErrorCode)>(
			Modio::Detail::RequestEmailAuthCodeOp(std::move(EmailAddress.InternalAddress)), Callback,
			Modio::Detail::Services::GetGlobalContext().get_executor());
	}

	void GetTermsOfUseAsync(Modio::AuthenticationProvider Provider, Modio::Language Locale,
					   std::function<void(Modio::ErrorCode, Modio::Optional<Modio::Terms> Terms)> Callback)
	{
		if (Modio::Detail::RequireSDKIsInitialized(Callback))
		{
			return asio::async_compose<std::function<void(Modio::ErrorCode, Modio::Optional<Modio::Terms>)>, void(Modio::ErrorCode, Modio::Optional<Modio::Terms>)>(
				Modio::Detail::GetTermsOfUseOp(Provider, Locale), Callback,
				Modio::Detail::Services::GetGlobalContext().get_executor());
		}
	}

	void AuthenticateUserExternalAsync(Modio::AuthenticationParams User, Modio::AuthenticationProvider Provider,
									   std::function<void(Modio::ErrorCode)> Callback)
	{
		if (Modio::Detail::RequireSDKIsInitialized(Callback) && Modio::Detail::RequireNotRateLimited(Callback) &&
			Modio::Detail::RequireUserIsNOTAuthenticated(Callback))
		{
			switch (Provider)
			{
				// @todo: Add Epic when we support Epic
				/*case AuthenticationProvider::Epic:
					AuthenticateUserWithEpicAsync(User, Callback);
					break;*/
				case AuthenticationProvider::GoG:
					Modio::Detail::AuthenticateUserByGoGAsync(User, Callback);
					break;
				case AuthenticationProvider::Itch:
					Modio::Detail::AuthenticateUserByItchAsync(User, Callback);
					break;
				case AuthenticationProvider::Steam:
					Modio::Detail::AuthenticateUserBySteamAsync(User, Callback);
					break;
				case AuthenticationProvider::XboxLive:
					Modio::Detail::AuthenticateUserByXBoxLiveAsync(User, Callback);
					break;
				case AuthenticationProvider::Switch:
					Modio::Detail::AuthenticateUserBySwitchIDAsync(User, Callback);
					break;
				case AuthenticationProvider::Discord:
					Modio::Detail::AuthenticateUserByDiscordAsync(User, Callback);
					break;
			}
		}
	}

	void AuthenticateUserEmailAsync(Modio::EmailAuthCode AuthenticationCode,
									std::function<void(Modio::ErrorCode)> Callback)
	{
		if (Modio::Detail::RequireUserIsNOTAuthenticated(Callback))
		{
			return asio::async_compose<std::function<void(Modio::ErrorCode)>, void(Modio::ErrorCode)>(
				Modio::Detail::AuthenticateUserByEmailOp(AuthenticationCode), Callback,
				Modio::Detail::Services::GetGlobalContext().get_executor());
		}
	}

	void ClearUserDataAsync(std::function<void(Modio::ErrorCode)> Callback)
	{
		if (Modio::Detail::RequireSDKIsInitialized(Callback) && Modio::Detail::RequireNotRateLimited(Callback) &&
			Modio::Detail::RequireUserIsAuthenticated(Callback))
		{
			return Modio::Detail::Services::GetGlobalService<Modio::Detail::UserDataService>().ClearUserDataAsync(
				Callback);
		}
	}

	Modio::Optional<Modio::User> QueryUserProfile()
	{
		return Modio::Detail::SDKSessionData::GetAuthenticatedUser();
	}

	void GetUserMediaAsync(Modio::AvatarSize AvatarSize,
						   std::function<void(Modio::ErrorCode, Modio::Optional<Modio::filesystem::path>)> Callback)
	{
		if (Modio::Detail::RequireSDKIsInitialized(Callback) && Modio::Detail::RequireNotRateLimited(Callback) &&
			Modio::Detail::RequireUserIsAuthenticated(Callback))
		{
			return asio::async_compose<std::function<void(Modio::ErrorCode, Modio::Optional<Modio::filesystem::path>)>,
									   void(Modio::ErrorCode, Modio::Optional<Modio::filesystem::path>)>(
				Modio::Detail::GetUserMediaOp(Modio::Detail::SDKSessionData::CurrentGameID(),
											  Modio::Detail::SDKSessionData::CurrentAPIKey(), AvatarSize),
				Callback, Modio::Detail::Services::GetGlobalContext().get_executor());
		}
	}

} // namespace Modio
