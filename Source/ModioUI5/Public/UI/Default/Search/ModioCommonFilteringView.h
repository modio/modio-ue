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
#include "ModioCommonFilteringView.generated.h"

class UModioCommonModTagGroupList;
class UPanelWidget;
class UModioCommonModTagList;

/**
 * @brief Filtering View that displays a list of tags that can be used to filter a mod listing
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI5_API UModioCommonFilteringView : public UModioCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UModioCommonFilteringView();

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI")
	TObjectPtr<UPanelWidget> FilteringTagsContainer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), Category = "Mod.io Common UI")
	TSubclassOf<UModioCommonModTagGroupList> TagGroupListClass;

#if WITH_EDITORONLY_DATA
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Mod.io Common UI|Preview")
	TArray<FModioModTagInfo> PreviewModTagInfoOptions;
#endif

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Mod.io Common UI")
	void ResetFiltering();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Mod.io Common UI")
	bool GetSelectedTagGroupValues(UPARAM(ref) TArray<FString>& OutSelectedTagGroupValues) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void AddModTagInfo(const FModioModTagInfo& ModTagInfo);
protected:
	virtual void NativeOnInitialized() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
public:
	virtual void SynchronizeProperties() override;
};
