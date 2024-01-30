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
#include "UObject/Interface.h"
#include "Types/ModioModInfo.h"
#include "Types/ModioModInfoList.h"
#include "Types/ModioModCollectionEntry.h"
#include "UObject/StrongObjectPtr.h"
#include "Core/ModioModInfoUI.h"
#include "Core/ModioModCollectionEntryUI.h"
#include "ModioCommonModListViewInterface.generated.h"

class UListView;

UINTERFACE()
class UModioCommonModListViewInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for a Mod List View that can be used in Mod.io UI
 */
class MODIOUI_API IModioCommonModListViewInterface
{
	GENERATED_BODY()

protected:
	virtual void SetModSelectionByID_Implementation(FModioModID ModID);
	virtual void SetModsFromModInfoList_Implementation(const FModioModInfoList& InList, bool bAddToExisting);
	virtual void SetModsFromModInfoArray_Implementation(const TArray<FModioModInfo>& InArray, bool bAddToExisting);
	virtual void SetModsFromModCollectionEntryArray_Implementation(const TArray<FModioModCollectionEntry>& InArray, bool bAddToExisting);
	virtual void RequestFullClearSelection_Implementation(bool bResetPreviouslySelected);
	virtual bool GetSelectedModItem_Implementation(bool bIncludePreviouslySelected, UObject*& OutModItem);
	virtual bool GetEntryWidgetFromItem_Implementation(UObject* InItem, UWidget*& OutEntryWidget);
	virtual UWidget* GetDesiredListFocusTarget_Implementation(bool bIncludePreviouslySelected, bool bIncludeFirstItem);

public:
	/**
	 * Gets the list view widget
	 * @return The list view widget
	 */
	virtual UListView* GetListView() const = 0;

	/**
	 * Sets the selection of the list view to the mod with the specified ID
	 * @param ModID The ID of the mod to select
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Modio Common UI|Mod List Interface")
	void SetModSelectionByID(FModioModID ModID);

	/**
	 * Sets the focus of the list view once it is populated
	 * @param bFocus Whether to focus the list view once it is populated
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Modio Common UI|Mod List Interface")
	void SetFocusOnceListIsPopulated(bool bFocus);

	/**
	 * Sets the mods to display in the list view, using a list of mod info
	 * @param InList The list of mods to display
	 * @param bAddToExisting Whether to add the mods to the existing list or replace the existing list
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Modio Common UI|Mod List Interface")
	void SetModsFromModInfoList(const FModioModInfoList& InList, bool bAddToExisting);

	/**
	 * Sets the mods to display in the list view, using an array of mod info
	 * @param InArray The array of mods to display
	 * @param bAddToExisting Whether to add the mods to the existing list or replace the existing list
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Modio Common UI|Mod List Interface")
	void SetModsFromModInfoArray(const TArray<FModioModInfo>& InArray, bool bAddToExisting);

	/**
	 * Sets the mods to display in the list view, using an array of mod collection entries
	 * @param InArray The array of mods to display
	 * @param bAddToExisting Whether to add the mods to the existing list or replace the existing list
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Modio Common UI|Mod List Interface")
	void SetModsFromModCollectionEntryArray(const TArray<FModioModCollectionEntry>& InArray, bool bAddToExisting);

	/**
	 * Requests that the list view fully clears its selection (making the list view have no selected mod)
	 * @param bResetPreviouslySelected Whether to reset the previously selected mod
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Modio Common UI|Mod List Interface")
	void RequestFullClearSelection(bool bResetPreviouslySelected);

	/**
	 * Gets the currently selected mod item
	 * @param bIncludePreviouslySelected Whether to include the previously selected mod in the search
	 * @param OutModItem The currently selected mod item
	 * @return Whether the currently selected mod item was found
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Modio Common UI|Mod List Interface")
	bool GetSelectedModItem(bool bIncludePreviouslySelected, UObject*& OutModItem);

	/**
	 * Gets the entry widget for the specified item
	 * @param InItem The item to get the entry widget for
	 * @param OutEntryWidget The entry widget for the specified item
	 * @return Whether the entry widget was found
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Modio Common UI|Mod List Interface")
	bool GetEntryWidgetFromItem(UObject* InItem, UWidget*& OutEntryWidget);

	/**
	 * Gets the desired focus target for the list view
	 * @param bIncludePreviouslySelected Whether to include the previously selected mod in the search
	 * @param bIncludeFirstItem Whether to include the first item in the search
	 * @return The desired focus target for the list view
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Modio Common UI|Mod List Interface")
	UWidget* GetDesiredListFocusTarget(bool bIncludePreviouslySelected, bool bIncludeFirstItem);

	/** The ID of the previously selected mod */
	TOptional<FModioModID> PreviouslySelectedModID;

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**
	 * Sets the list items
	 * @param InListItems The list items to set
	 * @param bAddToExisting Whether to add the items to the existing list or replace the existing list
	 */
	virtual void NativeSetListItems(const TArray<UObject*>& InListItems, bool bAddToExisting) {}

	/** Cached collection entries (used to avoid creating new collection entries every time the list is updated) */
	TMap<FModioModID, TStrongObjectPtr<UModioModCollectionEntryUI>> CachedCollectionEntries;

	/** Cached mod infos (used to avoid creating new mod infos every time the list is updated) */
	TMap<FModioModID, TStrongObjectPtr<UModioModInfoUI>> CachedModInfos;
};
