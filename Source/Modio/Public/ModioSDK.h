#pragma once

//#if PLATFORM_WINDOWS
//	// clang-format off
//	#include "Windows/AllowWindowsPlatformTypes.h"
//	#include "Windows/PreWindowsApi.h"
//	#include "Windows/AllowWindowsPlatformAtomics.h"
//// clang-format on
//#endif

#define MODIO_PLATFORM_UNREAL 1

// Ensure that we can run asio without any exceptions
namespace asio
{
	namespace detail
	{
		template<typename Exception>
		void throw_exception(const Exception& e)
		{
			checkf(false, TEXT("Asio threw a exception with the message %s"), *e.what());
		}
	} // namespace detail
} // namespace asio

#pragma push_macro("check")

#undef check

#pragma warning(push)
// These warnings are in third party libraries and we don't care about those
#pragma warning(disable : 4265 4996 4668 4191 4583 4582)
// These warnings is in our code, and should be fixed
#pragma warning(disable : 4063)
#include "modio/ModioSDK.h"



#undef MODIO_PLATFORM_UNREAL
#undef ASIO_NO_EXCEPTIONS

#pragma warning(pop)

#pragma pop_macro("check")

//#if PLATFORM_WINDOWS
//  // clang-format off
//	#include "Windows/PostWindowsApi.h"
//	#include "Windows/HideWindowsPlatformTypes.h"
//	#include "Windows/HideWindowsPlatformAtomics.h"
//  // clang-format on
//#endif
