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
	#include "modio/core/ModioInitializeOptions.h"
#endif

namespace Modio
{
	InitializeOptions::InitializeOptions() {}

	InitializeOptions::InitializeOptions(Modio::GameID GameID, Modio::ApiKey APIKey, Modio::Environment GameEnvironment,
										 Modio::Portal PortalInUse, Modio::UserHandleType User)
		: GameID(GameID),
		  APIKey(APIKey),
		  User(User),
		  PortalInUse(PortalInUse),
		  GameEnvironment(GameEnvironment) 
	{}
} // namespace Modio
