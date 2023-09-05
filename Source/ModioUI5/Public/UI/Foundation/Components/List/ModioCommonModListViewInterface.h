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
#include "ModioCommonModListViewInterface.generated.h"

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
	virtual void SetModsFromModInfoList_Implementation(const FModioModInfoList& InList, bool bAddToExisting);
	virtual void SetModsFromModInfoArray_Implementation(const TArray<FModioModInfo>& InArray, bool bAddToExisting);
	virtual void SetModsFromModCollectionEntryArray_Implementation(const TArray<FModioModCollectionEntry>& InArray, bool bAddToExisting);

public:
	UFUNCTION(BlueprintNativeEvent)
	void SetModSelectionByID(FModioModID ModID);

	UFUNCTION(BlueprintNativeEvent)
	void SetFocusOnceListIsPopulated(bool bFocus);
	
	UFUNCTION(BlueprintNativeEvent)
	void SetModsFromModInfoList(const FModioModInfoList& InList, bool bAddToExisting);
	
	UFUNCTION(BlueprintNativeEvent)
	void SetModsFromModInfoArray(const TArray<FModioModInfo>& InArray, bool bAddToExisting);
	
	UFUNCTION(BlueprintNativeEvent)
	void SetModsFromModCollectionEntryArray(const TArray<FModioModCollectionEntry>& InArray, bool bAddToExisting);

	UFUNCTION(BlueprintNativeEvent)
	void RequestFullClearSelection();

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void NativeSetListItems(const TArray<UObject*>& InListItems, bool bAddToExisting) {}
};
