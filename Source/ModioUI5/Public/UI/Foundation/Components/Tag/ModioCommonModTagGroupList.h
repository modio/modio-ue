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
class MODIOUI5_API UModioCommonModTagGroupList : public UModioCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tags Widget")
	TSubclassOf<UModioCommonModTagEntry> TagEntryClass;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonTextBlock> TagGroupNameTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI")
	TObjectPtr<UPanelWidget> TagsContainer;

#if WITH_EDITORONLY_DATA
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Mod.io Common UI")
	FModioModTagInfo PreviewModTagInfo;
#endif

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Tag Widget")
	void SetTagsGroup(const FModioModTagInfo& TagInfo);

	UFUNCTION(BlueprintNativeEvent, Category = "Tag Widget")
	bool SetSelectedTagGroupValues(const TArray<FString>& TagGroupValues, bool bSelect);

	UFUNCTION(BlueprintNativeEvent, Category = "Tag Widget")
	bool GetSelectedTagGroupValues(UPARAM(ref) TArray<FString>& OutSelectedTagGroupValues) const;

	UFUNCTION(BlueprintNativeEvent, Category = "Tag Widget")
	void ResetTagsSelection();

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Tag Widget")
	void UpdateSelection(UModioCommonModTagEntry* ModTagEntry, bool bIsSelected);

protected:
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
public:
	virtual void SynchronizeProperties() override;
};
