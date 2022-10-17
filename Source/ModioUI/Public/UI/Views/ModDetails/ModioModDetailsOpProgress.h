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

#include "ModioModDetailsOpProgress.generated.h"

/**
* Base class that stores details of a mod progress operation, keeping
* references for the progress bar and operation metadata.
**/
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

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	UModioProgressBar* ProgressBar;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidgetOptional))
	UModioRichTextBlock* TimeRemainingText;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	FText TimeRemainingTextFormat;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	FText SpeedTextFormat;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidgetOptional))
	UModioRichTextBlock* OperationSpeedText;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidgetOptional))
	UModioRichTextBlock* OperationProgressText;

	void UpdateProgress(const struct FModioModProgressInfo& ProgressInfo) override;

	void NativeOnModManagementEvent(FModioModManagementEvent Event) override;
};
