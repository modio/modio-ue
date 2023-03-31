#pragma once
#include "Widgets/SWindow.h"
#include "Templates/SharedPointer.h"

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
};