// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Misc/DateTime.h"
#include "Types/ModioModManagementEvent.h"
#include "UI/BaseWidgets/ModioProgressBar.h"
#include "UI/BaseWidgets/ModioRichTextBlock.h"
#include "UI/CommonComponents/ModioModManagementWidgetBase.h"

#include "ModioDownloadQueueOpProgress.generated.h"


DECLARE_DYNAMIC_DELEGATE(FOnDownloadOpComplete);

/**
 *
 */
UCLASS()
class MODIOUI_API UModioDownloadQueueOpProgress : public UModioModManagementWidgetBase
{
	GENERATED_BODY()
protected:
	FModioUnsigned64 PreviousProgressValue = FModioUnsigned64(0);
	FDateTime PreviousUpdateTime;

	void UpdateSpeed(FModioUnsigned64 DeltaBytes, double DeltaTime);
	void NativeOnSetDataSource() override;

	void SetPercent(float InPercent);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (BindWidget))
	UModioProgressBar* ProgressBar;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (BindWidget))
	UModioRichTextBlock* ModNameLabel;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (BindWidget))
	UModioRichTextBlock* OperationSpeedText;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (BindWidget))
	UModioRichTextBlock* OperationProgressText;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText SpeedFormatText;

	void UpdateProgress(const struct FModioModProgressInfo& ProgressInfo) override;

	void NativeOnModManagementEvent(FModioModManagementEvent Event) override;

	FOnDownloadOpComplete OnOperationCompleted;

public:
	UFUNCTION(BlueprintCallable)
	virtual FOnDownloadOpComplete& OperationCompletedDelegate();
};
