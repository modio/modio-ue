/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

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
#include "Objects/ModioStaticExecutionBase.h"

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

void UModioEditorUtilityFunctions::AddGettingStartedWidgetEntries(TSet<FModioGettingStartedEntry> NewEntries)
{
	if (UModioEditorSettings* EditorSettings = GetMutableDefault<UModioEditorSettings>())
	{
		EditorSettings->SubmoduleGettingStartedEntries.Append(NewEntries);
	}
}

bool UModioEditorUtilityFunctions::ExecuteStaticExecutor(UObject* Context,
														 TSubclassOf <UModioStaticExecutionBase> Executor,
														 FString Args)
{
	if (Executor)
	{
		if(UModioStaticExecutionBase* ExecutorRef = NewObject<UModioStaticExecutionBase>(Context, Executor))
		{
			return ExecutorRef->ExecuteAction(Args);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Tried to execute a Static Executor, but failed to create the object provided."));
			return false;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Tried to execute a Static Executor, but null class was provided."));
	return false;
}
