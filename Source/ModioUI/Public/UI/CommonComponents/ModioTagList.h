// Fill out your copyright notice in the Description page of Project Settings.

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
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int32 NumberOfPreviewTags = 1;
#endif

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int32 NumberOfTagsToShow = 1;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<FModioModTag> Tags;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText ExcessTagCountText;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UModioTagWidgetBase> TagWidgetClass;
	
	TOptional<FModioModTagOptions> TagOptions;

public:
	UFUNCTION(BlueprintCallable)
	void SetTags(TArray<FModioModTag> NewTags);
};
