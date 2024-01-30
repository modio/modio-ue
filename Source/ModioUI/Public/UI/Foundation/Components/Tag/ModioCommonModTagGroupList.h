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
#include "Types/ModioModTagInfo.h"
#include "ModioCommonModTagGroupList.generated.h"

class UModioCommonModTagEntry;
class UModioCommonTextBlock;
class UPanelWidget;

/**
 * Mod Tag Group List that displays a list of tag entries that belong to a single tag group and can be selected
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonModTagGroupList : public UModioCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	/** The tag entry class to use when creating tag entries */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tags Widget")
	TSubclassOf<UModioCommonModTagEntry> TagEntryClass;

	/** The tag group name text block */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonTextBlock> TagGroupNameTextBlock;

	/** Container for the tags */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI")
	TObjectPtr<UPanelWidget> TagsContainer;

#if WITH_EDITORONLY_DATA
	/** The tag group info to preview (in editor only) */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Mod.io Common UI")
	FModioModTagInfo PreviewModTagInfo;
#endif

public:
	/**
	 * Sets the tag group name
	 * @param TagInfo The tag group info
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Tag Widget")
	void SetTagsGroup(const FModioModTagInfo& TagInfo);

	/**
	 * Sets the selected tag group values
	 * @param TagGroupValues The tag group values
	 * @param bSelect Whether to select the tag group values
	 * @return Whether the tag group values were set
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Tag Widget")
	bool SetSelectedTagGroupValues(const TArray<FString>& TagGroupValues, bool bSelect);

	/**
	 * Gets the selected tag group values
	 * @param OutSelectedTagGroupValues The selected tag group values
	 * @return Whether the selected tag group values were retrieved
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Tag Widget")
	bool GetSelectedTagGroupValues(UPARAM(ref) TArray<FString>& OutSelectedTagGroupValues) const;

	/**
	 * Resets the selection of all tag entries, making them unselected
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Tag Widget")
	void ResetTagsSelection();

protected:
	/**
	 * Updates the selection of a tag entry
	 * @param ModTagEntry The tag entry to update
	 * @param bIsSelected Whether the tag entry is selected
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Tag Widget")
	void UpdateSelection(UModioCommonModTagEntry* ModTagEntry, bool bIsSelected);

protected:
	//~ Begin UCommonActivatableWidget Interface
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	//~ End UCommonActivatableWidget Interface
public:
	//~ Begin UWidget Interface
	virtual void SynchronizeProperties() override;
	//~ End UWidget Interface
};
