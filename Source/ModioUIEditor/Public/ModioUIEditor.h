/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#pragma once

#include "Modules/ModuleManager.h"
#include "Logging/LogMacros.h"

DECLARE_LOG_CATEGORY_EXTERN(ModioUIEditor, All, All);

class FModioUIEditor : public IModuleInterface
{
	/* Called when the module is loaded */
	virtual void StartupModule() override;

	/* Called when the module is unloaded */
	virtual void ShutdownModule() override;
};