// Fill out your copyright notice in the Description page of Project Settings.

#include "ModioEditorUtilityFunctions.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "CoreGlobals.h"
#include "Editor.h"
#include "LevelEditor.h"
#include "Misc/EngineVersionComparison.h"
#include "ModioEditorSettings.h"
#include "Modules/ModuleManager.h"
#include "Templates/UnrealTemplate.h"
#include "ISettingsModule.h"

void UModioEditorUtilityFunctions::SelectAssetsInContentBrowser(const TArray<FString>& AssetPaths)
{
	TGuardValue<bool> UnattendedScriptGuard(GIsRunningUnattendedScript, true);

	if (GEditor)
	{
		TArray<FAssetData> Assets;
		for (const FString& AssetPath : AssetPaths)
		{
			FString FailureReason;
			FAssetRegistryModule& AssetRegistryModule =
				FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
#if UE_VERSION_OLDER_THAN(5, 1, 0)
			FAssetData Asset = AssetRegistryModule.Get().GetAssetByObjectPath(*AssetPath);
#else
			FAssetData Asset = AssetRegistryModule.Get().GetAssetByObjectPath(FSoftObjectPath(AssetPath));
#endif
			if (Asset.IsValid())
			{
				Assets.Add(Asset);
			}
		}
		if (Assets.Num() > 0)
		{
			GEditor->SyncBrowserToObjects(Assets);
		}
	}
}

void UModioEditorUtilityFunctions::SetDisplayGettingStartedWidgetOnStartup(bool bNewValue)
{
	if (UModioEditorSettings* EditorSettings = GetMutableDefault<UModioEditorSettings>())
	{
		EditorSettings->bShowGettingStartedOnStartup = bNewValue;
		EditorSettings->SaveConfig();
	}
}

void UModioEditorUtilityFunctions::SetDisplayToolsMenuItem(bool bNewValue)
{
	if (UModioEditorSettings* EditorSettings = GetMutableDefault<UModioEditorSettings>())
	{
		EditorSettings->bDisplayToolsMenuItem = bNewValue;
		EditorSettings->SaveConfig();
	}
}

void UModioEditorUtilityFunctions::OpenTutorialBrowser()
{
	FLevelEditorModule& LevelEditorModule = FModuleManager::GetModuleChecked<FLevelEditorModule>(TEXT("LevelEditor"));
	LevelEditorModule.GetLevelEditorTabManager()->TryInvokeTab(FTabId("TutorialsBrowser"));
}

void UModioEditorUtilityFunctions::OpenModioSettings()
{
#if WITH_EDITOR
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->ShowViewer("Project", "Plugins", "mod.io");
	}
#endif
}
