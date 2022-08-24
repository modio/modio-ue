#include "ModioTestsModule.h"
#include "CoreMinimal.h"
#include "Engine/Engine.h"
#include "ModioTestSettings.h"
#include "PropertyEditorModule.h"

#if WITH_EDITOR
	#include "ISettingsModule.h"
#endif

#define LOCTEXT_NAMESPACE "FModioTestsModule"

void FModioTestsModule::StartupModule()
{
	RegisterSettings();
}

void FModioTestsModule::ShutdownModule()
{
	UnregisterSettings();
}

void FModioTestsModule::RegisterSettings()
{
#if WITH_EDITOR
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings(
			"Project", "Plugins", "mod.io unit tests", LOCTEXT("UnitTestsSettingsName", "mod.io unit tests"),
			LOCTEXT("UnitTestsSettingsDescription", "Configure the mod.io plugin unit tests"),
			GetMutableDefault<UModioTestSettings>());
	}
#endif
}
void FModioTestsModule::UnregisterSettings()
{
#if WITH_EDITOR
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "mod.io unit tests");
	}
#endif // WITH_EDITOR
}

IMPLEMENT_MODULE(FModioTestsModule, ModioTests)

#undef LOCTEXT_NAMESPACE