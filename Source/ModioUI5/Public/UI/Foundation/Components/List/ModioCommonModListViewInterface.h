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

// This class does not need to be modified.
UINTERFACE()
class UModioCommonModListViewInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MODIOUI5_API IModioCommonModListViewInterface
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
	virtual UListView* GetListView() const = 0;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Modio Common UI|Mod List Interface")
	void SetModSelectionByID(FModioModID ModID);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Modio Common UI|Mod List Interface")
	void SetFocusOnceListIsPopulated(bool bFocus);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Modio Common UI|Mod List Interface")
	void SetModsFromModInfoList(const FModioModInfoList& InList, bool bAddToExisting);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Modio Common UI|Mod List Interface")
	void SetModsFromModInfoArray(const TArray<FModioModInfo>& InArray, bool bAddToExisting);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Modio Common UI|Mod List Interface")
	void SetModsFromModCollectionEntryArray(const TArray<FModioModCollectionEntry>& InArray, bool bAddToExisting);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Modio Common UI|Mod List Interface")
	void RequestFullClearSelection(bool bResetPreviouslySelected);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Modio Common UI|Mod List Interface")
	bool GetSelectedModItem(bool bIncludePreviouslySelected, UObject*& OutModItem);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Modio Common UI|Mod List Interface")
	bool GetEntryWidgetFromItem(UObject* InItem, UWidget*& OutEntryWidget);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Modio Common UI|Mod List Interface")
	UWidget* GetDesiredListFocusTarget(bool bIncludePreviouslySelected, bool bIncludeFirstItem);

	TOptional<FModioModID> PreviouslySelectedModID;

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void NativeSetListItems(const TArray<UObject*>& InListItems, bool bAddToExisting) {}

	TMap<FModioModID, TStrongObjectPtr<UModioModCollectionEntryUI>> CachedCollectionEntries;
	TMap<FModioModID, TStrongObjectPtr<UModioModInfoUI>> CachedModInfos;
};
