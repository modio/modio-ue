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
#include "UI/EventHandlers/IModioUIModInfoReceiver.h"
#include "UI/EventHandlers/IModioUIUserChangedReceiver.h"
#include "UI/Foundation/Base/ModioCommonActivatableWidget.h"
#include "UI/Foundation/Components/List/ModioCommonModListViewInterface.h"
#include "UI/Interfaces/IModioUIAsyncOperationWidget.h"
#include "ModioCommonFilteredModListView.generated.h"

class UModioCommonTextBlock;
class UModioCommonButtonBase;
class UListView;
class UModioCommonFilteredModListViewStyle;
class UModioCommonErrorWithRetryWidget;

/**
 * Filtered Mod List View displays a list of mods that can be filtered
 * It supports styling through the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonFilteredModListView
	: public UModioCommonActivatableWidget,
	  public IModioCommonModListViewInterface,
	  public IModioUIModInfoReceiver,
	  public IModioUIAsyncOperationWidget,
	  public IModioUIUserChangedReceiver
{
	GENERATED_BODY()

public:
#if WITH_EDITORONLY_DATA
	/** Current page index in the list view (amongst all pages), for preview purposes */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (UIMin = 0, ClampMin = 0), Category = "Mod.io Common UI|Preview")
	int64 PreviewCurrentPageIndex = 2;

	/** Total number of mods available in the list view, for preview purposes */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (UIMin = 0, ClampMin = 0), Category = "Mod.io Common UI|Preview")
	int64 PreviewTotalMods = 256;

	/** Number of mods displayed per page in the list view, for preview purposes */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (UIMin = 0, ClampMin = 0), Category = "Mod.io Common UI|Preview")
	int64 PreviewPageSize = 100;

	/** Whether the list view should show the initial screen (the screen that is displayed by default when no search has been performed yet), for preview purposes */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Preview")
	bool bPreviewShowInitialScreen = false;

	/** Whether the list view should show the no results screen (the screen that is displayed when the search returns no results), for preview purposes */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Preview")
	bool bPreviewShowNoResults = false;

	/** Whether the list view should show the page navigation buttons, for preview purposes */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Preview")
	bool bPreviewShowPageNavigation = true;
#endif

public:
	/**
	 * Sets the style of the Filtered Mod List View within the Mod.io Common UI styling system
	 * @param InStyle The style to set
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Style")
	void SetStyle(UPARAM(DisplayName = "Style") TSubclassOf<UModioCommonFilteredModListViewStyle> InStyle);

protected:
	/** The style of the Filtered Mod List View within the Mod.io Common UI styling system */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), DisplayName = "Style", Category = "Mod.io Common UI")
	TSubclassOf<UModioCommonFilteredModListViewStyle> ModioStyle;

	/** The list view to display the mods */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, MustImplement = "/Script/ModioUI.ModioCommonModListViewInterface"), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UListView> ModList;

	/** The container that holds the initial screen (the screen that is displayed by default when no search has been performed yet) */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UPanelWidget> InitialScreenContainer;

	/** The container that holds the no results screen (the screen that is displayed when the search returns no results) */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UPanelWidget> NoResultsContainer;

	/** The container that holds the page navigation buttons */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UPanelWidget> PageNavigationContainer;

	/** The button to navigate to the previous page */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonButtonBase> PreviousPageButton;

	/** The button to navigate to the next page */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonButtonBase> NextPageButton;

	/** The text block that displays the current page index in the list view (amongst all pages) */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonTextBlock> CurrentPageTextBlock;

	/** The text block that displays the total number of pages in the list view */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonTextBlock> TotalPagesTextBlock;

	/** The text block that displays the total number of mods available in the list view */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonTextBlock> TotalModsTextBlock;

	/** Loader widget to display while the filtered mod list is loading */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, MustImplement = "/Script/ModioUICore.ModioUIAsyncHandlerWidget"), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UWidget> ModListLoader;

	/** Widget to display when there is an error loading the filtered mod list */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonErrorWithRetryWidget> ErrorWithRetryWidget;

public:
	DECLARE_MULTICAST_DELEGATE(FOnSetModsFromModInfoListStarted);
	/** Executed when the mod list is about to be populated */
	FOnSetModsFromModInfoListStarted OnSetModsFromModInfoListStarted;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSetModsFromModInfoListDynamicStarted);
	/** Executed when the mod list is about to be populated */
	UPROPERTY(BlueprintAssignable, Category = "Mod.io Common UI")
	FOnSetModsFromModInfoListDynamicStarted OnSetModsFromModInfoListDynamicStarted;

	DECLARE_MULTICAST_DELEGATE(FOnSetModsFromModInfoListFinished);
	/** Executed when the mod list has been populated */
	FOnSetModsFromModInfoListFinished OnSetModsFromModInfoListFinished;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSetModsFromModInfoListDynamicFinished);
	/** Executed when the mod list has been populated */
	UPROPERTY(BlueprintAssignable, Category = "Mod.io Common UI")
	FOnSetModsFromModInfoListDynamicFinished OnSetModsFromModInfoListDynamicFinished;

public:
	/**
	 * Gets the number of items selected in the list
	 * @return The number of items selected in the list
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	int32 GetNumItemsSelected() const;

	/**
	 * Gets the number of items in the list
	 * @return The number of items in the list
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	int32 GetNumItems() const;

protected:
	//~ Begin UModioCommonActivatableWidget Interface
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	virtual void NativeOnSetDataSource() override;
	virtual FString GetRequestIdentifier_Implementation() override;
	//~ End UModioCommonActivatableWidget Interface

	//~ Begin UUserWidget Interface
	virtual void NativePreConstruct() override;
	virtual void NativeOnInitialized() override;
public:
	virtual void SynchronizeProperties() override;
	//~ End UUserWidget Interface

protected:
	//~ Begin IModioUIUserChangedReceiver Interface
	virtual void NativeUserChanged(TOptional<FModioUser> NewUser) override;
	//~ End IModioUIUserChangedReceiver Interface
	
	//~ Begin IModioCommonModListViewInterface Interface
	virtual void SetModSelectionByID_Implementation(FModioModID ModID) override;
	virtual UListView* GetListView() const override { return ModList; }
	virtual void SetFocusOnceListIsPopulated_Implementation(bool bFocus) override;
	virtual void RequestFullClearSelection_Implementation(bool bResetPreviouslySelected) override;
	virtual void NativeSetListItems(const TArray<UObject*>& InListItems, bool bAddToExisting) override;
	//~ End IModioCommonModListViewInterface Interface

	//~ Begin IModioUIModInfoReceiver Interface
	virtual void NativeOnListAllModsRequestCompleted(FString RequestIdentifier, FModioErrorCode ErrorCode, TOptional<FModioModInfoList> List) override;
	//~ End IModioUIModInfoReceiver Interface

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void UpdateInputActions();

	/**
	 * Sets the visibility of the page navigation buttons
	 * @param bIsVisible Whether the page navigation buttons should be visible or not
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void SetPageNavigationVisibility(bool bIsVisible);

	/**
	 * Navigate to the previous page
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void HandlePreviousPageClicked();

	/**
	 * Navigate to the next page
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void HandleNextPageClicked();

	/**
	 * Sets the visibility of the initial screen (the screen that is displayed by default when no search has been performed yet)
	 * @param bIsVisible Whether the initial screen should be visible or not
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void SetInitialScreenVisibility(bool bIsVisible);

	/**
	 * Sets the visibility of the no results screen (the screen that is displayed when the search returns no results)
	 * @param bIsVisible Whether the no results screen should be visible or not
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void SetNoResultsVisibility(bool bIsVisible);

protected:
	/** Whether the list view has been searched before or not */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Mod.io Common UI|Properties")
	bool bHasSearchedBefore = false;

	/** The current page index in the list view (amongst all pages) */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Mod.io Common UI|Properties")
	int64 CurrentPageIndex = 0;

	/** The total number of mods available in the list view */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Mod.io Common UI|Properties")
	int64 TotalMods = 0;

	/** The number of mods displayed per page in the list view */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Mod.io Common UI|Properties")
	int64 PageSize = 0;

	/** The total number of pages in the list view */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Mod.io Common UI|Properties")
	int64 TotalPages = 0;
};
