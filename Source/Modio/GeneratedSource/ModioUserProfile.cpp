#ifdef MODIO_SEPARATE_COMPILATION
#include "modio/detail/userdata/ModioUserProfile.h"
#endif


namespace Modio
{
	namespace Detail
	{
		#ifdef MODIO_SEPARATE_COMPILATION
		Modio::Optional<std::string> OAuthToken::NoToken {};
		#endif
	}
}

 
