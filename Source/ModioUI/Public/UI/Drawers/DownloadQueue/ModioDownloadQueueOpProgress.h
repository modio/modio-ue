// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Misc/DateTime.h"
#include "Types/ModioModManagementEvent.h"
#include "UI/BaseWidgets/ModioProgressBar.h"
#include "UI/BaseWidgets/ModioRichTextBlock.h"
#include "UI/CommonComponents/ModioModManagementWidgetBase.h"
#include "UI/CommonComponents/ModioRichTextButton.h"

#include "ModioDownloadQueueOpProgress.generated.h"


DECLARE_DYNAMIC_DELEGATE(FOnDownloadOpComplete);

/**
 *
 */
UCLASS()
class MODIOUI_API UModioDownloadQueueOpProgress : public UModioModManagementWidgetBase
{
	GENERATED_BODY()
public:
	void UpdateProgress(const struct FModioModProgressInfo& ProgressInfo) override;

protected:
	FModioUnsigned64 PreviousProgressValue = FModioUnsigned64(0);
	FDateTime PreviousUpdateTime;

	void UpdateSpeed(FModioUnsigned64 DeltaBytes, double DeltaTime);
	void NativeOnSetDataSource() override;
	void NativeOnInitialized() override;

	UFUNCTION()
	void OnUnsubscribeClicked();

	void SetPercent(float InPercent);

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioProgressBar* ProgressBar;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioRichTextBlock* ModNameLabel;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioRichTextBlock* OperationSpeedText;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioRichTextBlock* OperationProgressText;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioRichTextButton* UnsubscribeButton;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widgets")
	FText SpeedFormatText;

	void NativeOnModManagementEvent(FModioModManagementEvent Event) override;

	FOnDownloadOpComplete OnOperationCompleted;

public:
	UFUNCTION(BlueprintCallable, Category="ModioDownloadQueueOpProgress")
	virtual FOnDownloadOpComplete& OperationCompletedDelegate();
};
