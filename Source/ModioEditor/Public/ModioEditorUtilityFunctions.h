/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityLibrary.h"
#include "ModioEditorUtilityFunctions.generated.h"

/**
 *
 */
UCLASS()
class MODIOEDITOR_API UModioEditorUtilityFunctions : public UEditorUtilityLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Modio|EditorUtilities")
	static void SelectAssetsInContentBrowser(const TArray<FString>& AssetPaths);

	UFUNCTION(BlueprintCallable, Category = "Modio|EditorUtilities")
	static void SetDisplayGettingStartedWidgetOnStartup(bool bNewValue);

	UFUNCTION(BlueprintCallable, Category = "Modio|EditorUtilities")
	static void SetDisplayToolsMenuItem(bool bNewValue);

	UFUNCTION(BlueprintCallable, Category = "Modio|EditorUtilities")
	static void OpenTutorialBrowser();

	UFUNCTION(BlueprintCallable, Category = "Modio|EditorUtilities")
	static void OpenModioSettings();

	UFUNCTION(BlueprintCallable, Category = "Modio|EditorUtilities")
	static void AddGettingStartedWidgetEntries(TSet<struct FModioGettingStartedEntry> NewEntries);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "Context", Category = "Modio|EditorUtilities"))
	static bool ExecuteStaticExecutor(UObject* Context, TSubclassOf<class UModioStaticExecutionBase> Executor, FString Args);
};
