#pragma once

// This makes missing a value in the switch statement a fatal error, is disabled after our conversion routines
#define MODIO_BEGIN_CONVERT_SWITCHES \
	__pragma(warning(push)) \
	__pragma(warning(error : 4062))

#define MODIO_END_CONVERT_SWITCHES \
	__pragma(warning(pop))
