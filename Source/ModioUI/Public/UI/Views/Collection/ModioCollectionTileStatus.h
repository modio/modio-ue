// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Misc/DateTime.h"
#include "Types/ModioModManagementEvent.h"
#include "UI/BaseWidgets/ModioProgressBar.h"
#include "UI/BaseWidgets/ModioRichTextBlock.h"
#include "UI/CommonComponents/ModioModManagementWidgetBase.h"

#include "ModioCollectionTileStatus.generated.h"
/**
 *
 */
UCLASS()
class MODIOUI_API UModioCollectionTileStatus : public UModioModManagementWidgetBase
{
	GENERATED_BODY()
protected:
	int64 PreviousProgressValue = 0;
	FDateTime PreviousUpdateTime;

	void NativeOnSetDataSource() override;

	void SetPercent(float InPercent);
	
	// These could possibly be moved into the global style
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText PendingLabelText;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText DownloadingLabelText;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText ExtractingLabelText;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText SubscribedLabelText;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (BindWidget))
	UModioRichTextBlock* StatusText;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (BindWidget))
	UModioProgressBar* ProgressBar;

	void UpdateProgress(const struct FModioModProgressInfo& ProgressInfo) override;

	void NativeOnModManagementEvent(FModioModManagementEvent Event) override;
};
