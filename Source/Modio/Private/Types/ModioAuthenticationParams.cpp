#include "Types/ModioAuthenticationParams.h"
#include "Internal/ModioConvert.h"
#include "GenericPlatform/GenericPlatformHttp.h"

FModioAuthenticationParams::operator Modio::AuthenticationParams() const
{
	Modio::AuthenticationParams Params;
	Params.AuthToken = ToSTD(FGenericPlatformHttp::UrlEncode(AuthToken));
	Params.UserEmail = UserEmail.TrimStartAndEnd().IsEmpty() ? Modio::Optional<std::string>(ToSTD(UserEmail)) : Modio::Optional<std::string>();
	Params.bUserHasAcceptedTerms = bUserHasAcceptedTerms;

	return Params;
}
