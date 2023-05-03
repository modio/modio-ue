/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#pragma once

#include "CoreMinimal.h"
#include "Types/ModioModTag.h"
#include "UI/BaseWidgets/ModioWrapBox.h"
#include "UI/CommonComponents/ModioTagWidgetBase.h"

#include "ModioTagList.generated.h"

/**
 *
 */
UCLASS()
class MODIOUI_API UModioTagList : public UModioWrapBox
{
	GENERATED_BODY()

protected:
	virtual void SynchronizeProperties() override;

	void UpdateTagWidgets();
	FString GetTagGroupNameForTagValue(FString TagValue);

#if WITH_EDITORONLY_DATA
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	int32 NumberOfPreviewTags = 1;
#endif

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	int32 NumberOfTagsToShow = 1;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	TArray<FModioModTag> Tags;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	FText ExcessTagCountText;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	TSubclassOf<UModioTagWidgetBase> TagWidgetClass;

	TOptional<FModioModTagOptions> TagOptions;

public:
	UFUNCTION(BlueprintCallable, Category = "Widgets")
	void SetTags(TArray<FModioModTag> NewTags);

	TSubclassOf<UModioTagWidgetBase> GetTagWidgetClass();
};
