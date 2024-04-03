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
#include "Core/ModioFilterParamsUI.h"
#include "UI/Foundation/Base/ModioCommonActivatableWidget.h"
#include "UI/Foundation/Base/ModBrowser/ModioCommonModListBase.h"
#include "ModioCommonSearchResultsView.generated.h"

class UModioCommonSearchResultsViewStyle;
class UModioCommonModTagList;
class UModioCommonTextBlock;
class UModioCommonFilteredModListView;
enum class EModioSortFieldType : uint8;
enum class EModioSortDirection : uint8;

/**
 * Search Results View that displays a list of mods that match the search criteria (search tags, filtering and sorting)
 * It supports styling through the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonSearchResultsView : public UModioCommonModListBase
{
	GENERATED_BODY()

public:
#if WITH_EDITORONLY_DATA
	/** Preview of the filter params (editor only) */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Preview")
	FModioModCategoryParams PreviewFilterParams = FModioModCategoryParams(FModioFilterParams().NameContains(TEXT("Keyword Example")).WithTags({ TEXT("Tag 1"), TEXT("Tag 2"), TEXT("Tag 3"), TEXT("Tag 4"), TEXT("Tag 5") }));
#endif

public:
	/**
	 * Sets the style of the Search Results View within the Mod.io Common UI styling system
	 * @param InStyle The style to set
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Style")
	void SetStyle(UPARAM(DisplayName = "Style") TSubclassOf<UModioCommonSearchResultsViewStyle> InStyle);

protected:
	/** The style of the Search Results View within the Mod.io Common UI styling system */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), DisplayName = "Style", Category = "Mod.io Common UI")
	TSubclassOf<UModioCommonSearchResultsViewStyle> ModioStyle;

public:
	/** Filtered mod list view that displays the results of the search */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonFilteredModListView> FilteredModListView;

protected:
	/** Keywords label text block */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonTextBlock> KeywordsLabelTextBlock;

	/** Keywords details text block (the keywords themselves) */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonTextBlock> KeywordsDetailsTextBlock;

	/** Categories label text block */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonTextBlock> CategoriesLabelTextBlock;

	/** Categories list (the categories themselves, which are groups of tags) */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonModTagList> CategoriesList;

	/** Button that allows the user to edit the search criteria */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonButtonBase> EditSearchButton;

	/** Filter counter text block (displays the number of mods that match the search criteria) */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonTextBlock> FilterCounterTextBlock;

public:
	/**
	 * Shows the search view (the filtering options)
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void ShowSearchView();

	/**
	 * Gets the number of applied filters
	 * 
	 * @param NumOfAppliedFilters The number of applied filters
	 * @return True if the number of applied filters was successfully retrieved, false otherwise
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	bool GetNumOfAppliedFilters(int32& NumOfAppliedFilters) const;

protected:
	//~ Begin UCommonActivatableWidget Interface
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	//~ End UCommonActivatableWidget Interface
public:
	//~ Begin UWidget Interface
	virtual void SynchronizeProperties() override;
	//~ End UWidget Interface
protected:
	//~ Begin UUserWidget Interface
	virtual void NativeOnInitialized() override;
	//~ End UUserWidget Interface

	//~ Begin UModioCommonActivatableWidget Interface
	virtual void NativeOnSetDataSource() override;
	//~ End UModioCommonActivatableWidget Interface

	/** Currently applied filter params */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Mod.io Common UI|Data")
	FModioModCategoryParams AppliedFilterParams;

public:
	/** Whether the search has been performed at least once or not */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Mod.io Common UI|Data")
	bool bWasEverPopulated = false;

	/**
	 * Cached default filter params
	 * Can be used a reference to compare against the current filter params, for example, to determine the number of applied filters
	 */
	UPROPERTY(Transient, BlueprintReadWrite, Category = "Mod.io Common UI")
	TObjectPtr<UModioFilterParamsUI> CachedDefaultFilterParams;
};
