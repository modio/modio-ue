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
 * @brief Displays a list of tags (group of tags) for a mod
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI5_API UModioCommonModTagList : public UModioCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "Tags Widget")
	TSubclassOf<UModioCommonModTagEntry> TagEntryClass;

	UPROPERTY(BlueprintReadWrite, Category = "Tags Widget", meta = (BindWidgetOptional))
	TObjectPtr<UPanelWidget> TagsContainer;

protected:
	virtual UWidget* NativeGetDesiredFocusTarget() const override;

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Tags Widget")
	void SetTags(const TArray<FModioModTag>& ModTags);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Tags Widget")
	void SetTagsString(const TArray<FString>& Tags);
};
