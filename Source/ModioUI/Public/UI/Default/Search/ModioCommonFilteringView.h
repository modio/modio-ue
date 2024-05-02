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
#include "Types/ModioModTagInfo.h"
#include "ModioCommonFilteringView.generated.h"

class UModioCommonModTagGroupList;
class UPanelWidget;
class UModioCommonModTagList;

/**
 * Filtering View that displays a list of tags that can be used to filter a mod listing
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonFilteringView : public UModioCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UModioCommonFilteringView();

protected:
	/** Container for the tag group list (filtering options) */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI")
	TObjectPtr<UPanelWidget> FilteringTagsContainer;

	/** The tag group list class */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), Category = "Mod.io Common UI")
	TSubclassOf<UModioCommonModTagGroupList> TagGroupListClass;

	/**
	 * The maximum number of tags that can be selected
	 * If the number of selected tags exceeds this value, the oldest selected tags will be skipped in GetFilterParamsWrapper
	 * 20 by default (mod.io API limit)
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mod.io Common UI")
	int32 MaxNumOfTags = 20;

#if WITH_EDITORONLY_DATA
	/** Preview of the mod tag info options (editor only) */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Mod.io Common UI|Preview")
	TArray<FModioModTagInfo> PreviewModTagInfoOptions;
#endif

public:
	/**
	 * Resets the filtering options to the default values
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Mod.io Common UI")
	void ResetFiltering();

	/**
	 * Zeros out the filtering options, making all tags unselected
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Mod.io Common UI")
	void ZeroOutFiltering();

	/**
	 * Sets the selected tag group values (filtering options)
	 * @param TagGroupValues The tag group values
	 * @param bSelect Whether to select the tag group values
	 * @return Whether the tag group values were set
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Mod.io Common UI")
	bool SetSelectedTagGroupValues(const TArray<FString>& TagGroupValues, bool bSelect);

	/**
	 * Gets the selected tag group values (filtering options)
	 * @param OutSelectedTagGroupValues The selected tag group values
	 * @return Whether the selected tag group values were retrieved
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Mod.io Common UI")
	bool GetSelectedTagGroupValues(UPARAM(ref) TArray<FString>& OutSelectedTagGroupValues) const;

	/**
	 * Gets the filter params wrapper for the filtering options (easier to modify in Blueprints)
	 * @return The filter params wrapper
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Mod.io Common UI")
	FModioModCategoryParams GetFilterParamsWrapper() const;

	/**
	 * Adds a tag group list to the filtering view
	 * @param ModTagInfo The mod tag info
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void AddModTagInfo(const FModioModTagInfo& ModTagInfo);

	/**
	 * Synchronizes the filter params with the previously selected tag group values
	 * @param PreviouslySelectedTagGroupValues The previously selected tag group values
	 * @param bResetToDefault Whether to reset the filter params to the default values
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void SynchronizeFilterParams(const TArray<FString>& PreviouslySelectedTagGroupValues, bool bResetToDefault = false);
protected:
	//~ Begin UCommonActivatableWidget Interface
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	//~ End UCommonActivatableWidget Interface
public:
	//~ Begin UWidget Interface
	virtual void SynchronizeProperties() override;
	//~ End UWidget Interface
};
