// Fill out your copyright notice in the Description page of Project Settings.

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
};
