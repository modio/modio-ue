// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Misc/DateTime.h"
#include "Types/ModioModManagementEvent.h"
#include "UI/BaseWidgets/ModioProgressBar.h"
#include "UI/BaseWidgets/ModioRichTextBlock.h"
#include "UI/CommonComponents/ModioModManagementWidgetBase.h"

#include "ModioModDetailsOpProgress.generated.h"
/**
 *
 */
UCLASS()
class MODIOUI_API UModioModDetailsOpProgress : public UModioModManagementWidgetBase
{
	GENERATED_BODY()
protected:
	FModioUnsigned64 PreviousProgressValue = FModioUnsigned64(0);
	FDateTime PreviousUpdateTime;
	virtual void NativeOnInitialized() override;
	virtual void NativeOnSetDataSource() override;
	void UpdateSpeed(FModioUnsigned64 DeltaBytes, double DeltaTime);

	void SetPercent(float InPercent);
	void UpdateTimeRemaining(FModioUnsigned64 ProgressSinceLastUpdate, FModioUnsigned64 AmountRemaining,
							 double SecondsSinceLastUpdate);

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioProgressBar* ProgressBar;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidgetOptional))
	UModioRichTextBlock* TimeRemainingText;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets")
	FText TimeRemainingTextFormat;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets")
	FText SpeedTextFormat;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidgetOptional))
	UModioRichTextBlock* OperationSpeedText;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidgetOptional))
	UModioRichTextBlock* OperationProgressText;

	void UpdateProgress(const struct FModioModProgressInfo& ProgressInfo) override;

	void NativeOnModManagementEvent(FModioModManagementEvent Event) override;
};
