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
#include "Types/ModioModInfo.h"

#include "ModioCommonModTagList.generated.h"

class UModioCommonWrapBox;
class UModioCommonModTagEntry;

/**
 * Displays a list of tags (group of tags) for a mod
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonModTagList : public UModioCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	/** The tag entry class to use when creating tag entries */
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "Tags Widget")
	TSubclassOf<UModioCommonModTagEntry> TagEntryClass;

	/** Container for the tags */
	UPROPERTY(BlueprintReadWrite, Category = "Tags Widget", meta = (BindWidgetOptional))
	TObjectPtr<UPanelWidget> TagsContainer;

protected:
	//~ Begin UWidget Interface
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	//~ End UWidget Interface

public:
	/**
	 * Sets the tags to display
	 * @param ModTags The tags to display
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Tags Widget")
	void SetTags(const TArray<FModioModTag>& ModTags);

	/**
	 * Sets the tags to display (as strings)
	 * @param Tags The tags to display
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Tags Widget")
	void SetTagsString(const TArray<FString>& Tags);
};
