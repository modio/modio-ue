/*
 *  Copyright (C) 2025 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once
#include "Templates/SharedPointer.h"
#include "Widgets/SWindow.h"

#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(ModioEditor, All, All);

class FModioEditor : public IModuleInterface
{
public:
	/* Called when the module is loaded */
	virtual void StartupModule() override;

	/* Called when the module is unloaded */
	virtual void ShutdownModule() override;
	static void DisplayGettingStarted_PostMainFrame(TSharedPtr<SWindow>, bool);

	static void DisplayGettingStarted();

	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();

private:
	void RegisterMenus();
	TSharedRef<class SWidget> PopulateComboButton(TSharedPtr<class FUICommandList> Commands);
	TSharedPtr<class FUICommandList> PluginCommands;
};