#include "ModioEditor.h"
#include "ModioEditorSettings.h"

#if WITH_EDITOR
	#include "ISettingsModule.h"
	#include "Modules/ModuleManager.h"
	#include "EditorUtilityWidgetBlueprint.h"
	#include "Editor.h"
	#include "Interfaces/IMainFrameModule.h"
	#include "EditorUtilitySubsystem.h"
#endif

DEFINE_LOG_CATEGORY(ModioEditor);

#define LOCTEXT_NAMESPACE "FModioEditor"

void FModioEditor::StartupModule()
{
#if WITH_EDITOR
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings(
			"Project", "Plugins", "mod.io Editor", LOCTEXT("EditorSettingsName", "mod.io Editor"),
			LOCTEXT("EditorSettingsDescription", "Configure the mod.io plugin's editor-specific behaviour"),
			GetMutableDefault<UModioEditorSettings>());
	}
	if (GetMutableDefault<UModioEditorSettings>()->bShowGettingStartedOnStartup)
	{
		IMainFrameModule& MainFrameModule = IMainFrameModule::Get();
		if (MainFrameModule.IsWindowInitialized())
		{
			DisplayGettingStarted();
		}
		else
		{
			MainFrameModule.OnMainFrameCreationFinished().AddStatic(&FModioEditor::DisplayGettingStarted_PostMainFrame);
		}
	}
#endif
}

void FModioEditor::ShutdownModule()
{
#if WITH_EDITOR
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "mod.io Editor");
	}
#endif // WITH_EDITOR
}

void FModioEditor::DisplayGettingStarted_PostMainFrame(TSharedPtr<SWindow>, bool)
{
	DisplayGettingStarted();
}

void FModioEditor::DisplayGettingStarted()
{
#if WITH_EDITOR

	UEditorUtilityWidgetBlueprint* Blueprint =
		GetMutableDefault<UModioEditorSettings>()->GettingStartedWidget.LoadSynchronous();

	if (Blueprint)
	{
		if (UEditorUtilitySubsystem* Subsystem = GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>())
		{
			Subsystem->SpawnAndRegisterTab(Blueprint);
		}
	}
#endif
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FModioEditor, ModioEditor)