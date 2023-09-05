/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "CoreMinimal.h"
#include "UI/Foundation/Base/ModioCommonActivatableWidget.h"
#include "UI/Foundation/Base/ModBrowser/ModioCommonModListBase.h"
#include "ModioCommonSearchResultsView.generated.h"

class UModioCommonSearchResultsViewStyle;
class UModioCommonModTagList;
class UModioCommonTextBlock;
class UModioCommonFilteredModListView;
enum class EModioSortDirection : uint8;

/**
 * @brief Search Results View that displays a list of mods that match the search criteria (search tags, filtering and sorting)
 * It supports styling through the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI5_API UModioCommonSearchResultsView : public UModioCommonModListBase
{
	GENERATED_BODY()

public:
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (UIMin = 0, ClampMin = 0), Category = "Mod.io Common UI|Preview")
	FString PreviewKeywords = TEXT("Keyword Example");

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (UIMin = 0, ClampMin = 0), Category = "Mod.io Common UI|Preview")
	TArray<FString> PreviewTags = { TEXT("Tag 1"), TEXT("Tag 2"), TEXT("Tag 3"), TEXT("Tag 4"), TEXT("Tag 5") };
#endif

public:
	/**
	 * @brief Sets the style of the Search Results View within the Mod.io Common UI styling system
	 * @param InStyle The style to set
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Style")
	void SetStyle(UPARAM(DisplayName = "Style") TSubclassOf<UModioCommonSearchResultsViewStyle> InStyle);

protected:
	/** @brief The style of the Search Results View within the Mod.io Common UI styling system */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), DisplayName = "Style", Category = "Mod.io Common UI")
	TSubclassOf<UModioCommonSearchResultsViewStyle> ModioStyle;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonFilteredModListView> FilteredModListView;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonTextBlock> KeywordsLabelTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonTextBlock> KeywordsDetailsTextBlock;

	/** Stored property to tags label text block */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonTextBlock> CategoriesLabelTextBlock;

	/** Stored property to tags widget */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonModTagList> CategoriesList;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonButtonBase> EditSearchButton;

protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void ShowSearchView();

protected:
	virtual void NativeOnActivated() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
public:
	virtual void SynchronizeProperties() override;
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeOnSetDataSource() override;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Mod.io Common UI|Data")
	EModioSortDirection Direction;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Mod.io Common UI|Data")
	FString Keywords;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Mod.io Common UI|Data")
	TArray<FString> Tags;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Mod.io Common UI|Data")
	bool bWasEverPopulated = false;
};
