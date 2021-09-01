/* 
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *  
 *  This file is part of the mod.io SDK.
 *  
 *  Distributed under the MIT License. (See accompanying file LICENSE or 
 *   view online at <https://github.com/modio/modio-sdk/blob/main/LICENSE>)
 *  
 */

#ifdef MODIO_SEPARATE_COMPILATION
	#include "modio/detail/userdata/ModioUserDataContainer.h"
#endif

namespace Modio
{
	namespace Detail
	{
		void UserDataContainer::ResetUserData()
		{
			UserSubscriptions = {};
			DeferredUnsubscriptions = {};
			AuthenticatedProfile = {};
		}

		void UserDataContainer::InitializeForAuthenticatedUser(Modio::Detail::AuthenticatedUser AuthenticatedUser,
															   Modio::Detail::OAuthToken AuthToken)
		{
			ResetUserData();
			AuthenticatedProfile = Modio::Detail::ProfileData {AuthenticatedUser, AuthToken};
		}

		const Modio::Optional<Modio::User> UserDataContainer::GetAuthenticatedUser() const
		{
			return AuthenticatedProfile.map_or(&Modio::Detail::ProfileData::GetUser, Modio::Optional<Modio::User> {});
		}

		const Modio::Optional<Modio::Detail::Avatar> UserDataContainer::GetAuthenticatedUserAvatar() const
		{
			return AuthenticatedProfile.map_or(&Modio::Detail::ProfileData::GetAvatar,
											   Modio::Optional<Modio::Detail::Avatar> {});
		}

		const Modio::Optional<Modio::Detail::OAuthToken> UserDataContainer::GetAuthenticationToken() const
		{
			if (AuthenticatedProfile)
			{
				Modio::Optional<Modio::Detail::OAuthToken> Token = AuthenticatedProfile->GetToken();
				if (Token && Token->GetTokenState() == OAuthTokenState::Valid)
				{
					return Token;
				}
			}
			return {};
		}

		const bool UserDataContainer::IsValid() const
		{
			return AuthenticatedProfile.has_value();
		}

	} // namespace Detail
} // namespace Modio
