#include "ModioEditor.h"
#include "ModioEditorSettings.h"
#include "Runtime/Launch/Resources/Version.h"
#include "Misc/EngineVersionComparison.h"

#if WITH_EDITOR
	#include "ISettingsModule.h"
	#include "Modules/ModuleManager.h"
	#include "EditorUtilityWidgetBlueprint.h"
	#include "Editor.h"
	#include "Interfaces/IMainFrameModule.h"
	#include "EditorUtilitySubsystem.h"

	#include "ModioEditorWindowStyle.h"
	#include "ModioEditorWindowCommands.h"
	#include "Libraries/ModioSDKLibrary.h"
	#include "ToolMenus.h"
	#include "Interfaces/IPluginManager.h"
	#include "WindowManager.h"
	#include "Misc/MessageDialog.h"
	#include "GenericPlatform/GenericPlatformMisc.h"
#endif

#include "ModioSettings.h"
#include "DetailCustomizations/ModioSettingsDetails.h"

DEFINE_LOG_CATEGORY(ModioEditor);

#define LOCTEXT_NAMESPACE "FModioEditor"

void FModioEditor::StartupModule()
{
#if WITH_EDITOR

	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomClassLayout(
		UModioSettings::StaticClass()->GetFName(),
		FOnGetDetailCustomizationInstance::CreateStatic(&FModioSettingsDetails::MakeInstance));

	PropertyModule.NotifyCustomizationModuleChanged();

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

	FModioEditorWindowStyle::Initialize();
	FModioEditorWindowStyle::ReloadTextures();

	FModioEditorWindowCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(FModioEditorWindowCommands::Get().OpenPluginWindow, FExecuteAction::CreateRaw(this, &FModioEditor::PluginButtonClicked), FCanExecuteAction());

	if (GetMutableDefault<UModioEditorSettings>()->bDisplayToolsMenuItem)
	{
		UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FModioEditor::RegisterMenus));
	}

	WindowManager::Get().RegisterObjectCustomizations();
#endif
}

void FModioEditor::ShutdownModule()
{
#if WITH_EDITOR

	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "mod.io Editor");
	}

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FModioEditorWindowStyle::Shutdown();

	FModioEditorWindowCommands::Unregister();

	WindowManager::Get().UnregisterObjectCustomizations();
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


void FModioEditor::PluginButtonClicked()
{
	FModioInitializeOptions InitializeOptions = UModioSDKLibrary::GetProjectInitializeOptionsForSessionId(FString("ModioUnrealEditor"));
	if (!InitializeOptions.GameId.IsValid() || !InitializeOptions.ApiKey.IsValid())
	{
		const FText Title = FText::FromString("mod.io");
		const FText Message = FText::FromString("Please specify a valid Game Id and Api Key in 'Project Settings->Plugins->mod.io'");

		UE_LOG(LogTemp, Error, TEXT("%s"), *Message.ToString());
#if UE_VERSION_OLDER_THAN(5, 3, 0)
		EAppReturnType::Type UserSelection = FMessageDialog::Open(EAppMsgType::Ok, Message, &Title);
		#else
		EAppReturnType::Type UserSelection = FMessageDialog::Open(EAppMsgType::Ok, Message, Title);
		#endif
		if (UserSelection == EAppReturnType::Ok)
		{
			return;
		}
	}

	TSharedPtr<SWindow> ModioWindow = WindowManager::Get().GetWindow();
	if (ModioWindow.IsValid())
	{
		ModioWindow->ShowWindow();
	}
}

void FModioEditor::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FModioEditorWindowCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}
	{
#if ENGINE_MAJOR_VERSION >= 5
		FName ToolBarName = "LevelEditor.LevelEditorToolBar.PlayToolBar";
		FName ExtensionPoint = "Play";
#else
		FName ToolBarName = "LevelEditor.LevelEditorToolBar";
		FName ExtensionPoint = "Settings";
#endif
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu(ToolBarName);
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection(ExtensionPoint);
			{
				FToolMenuEntry& EntryComboButton = Section.AddEntry(FToolMenuEntry::InitComboButton("Modio", FUIAction(),
					FNewToolMenuChoice(FOnGetContent::CreateRaw(this, &FModioEditor::PopulateComboButton, PluginCommands)),
					LOCTEXT("modio", "mod.io"), 
					LOCTEXT("modio", "mod.io"),
					FModioEditorWindowCommands::Get().OpenPluginWindow->GetIcon(), false));
				EntryComboButton.SetCommandList(PluginCommands);
			}
		}
	}
}

TSharedRef<SWidget> FModioEditor::PopulateComboButton(TSharedPtr<FUICommandList> Commands)
{
	FMenuBuilder MenuBuilder(true, Commands);
	MenuBuilder.AddMenuEntry(FModioEditorWindowCommands::Get().OpenPluginWindow);
	return MenuBuilder.MakeWidget();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FModioEditor, ModioEditor)