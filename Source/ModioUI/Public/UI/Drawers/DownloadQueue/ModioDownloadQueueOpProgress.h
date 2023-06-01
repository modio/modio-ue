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

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	UModioRichTextButton* UnsubscribeButton;

	EModioModManagementEventType CurrentStatus = EModioModManagementEventType::Installed;

protected:
	float truncateDivider = 1.0f;

	FModioUnsigned64 PreviousProgressValue = FModioUnsigned64(0);
	FDateTime PreviousUpdateTime;
	
	void UpdateSpeed(FModioUnsigned64 DeltaBytes, double DeltaTime);
	void NativeOnSetDataSource() override;
	void NativeOnInitialized() override;

	UFUNCTION()
	void OnUnsubscribeClicked();

	void SetPercent(float InPercent);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	UModioProgressBar* ProgressBar;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	UModioRichTextBlock* ModNameLabel;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	UModioRichTextBlock* OperationSpeedText;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	UModioRichTextBlock* OperationProgressText;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Localization")
	FText SpeedFormatText;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Localization")
	FText ProgressFormatText = FText::FromString(TEXT("{Progress} of {Total}"));

	void NativeOnModManagementEvent(FModioModManagementEvent Event) override;

	FOnDownloadOpComplete OnOperationCompleted;

public:
	UFUNCTION(BlueprintCallable, Category = "ModioDownloadQueueOpProgress")
	virtual FOnDownloadOpComplete& OperationCompletedDelegate();
};
