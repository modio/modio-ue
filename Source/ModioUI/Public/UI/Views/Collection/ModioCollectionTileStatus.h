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
#include "UI/BaseWidgets/ModioSizeBox.h"
#include "UI/CommonComponents/ModioModManagementWidgetBase.h"

#include "ModioCollectionTileStatus.generated.h"

/**
* Modio derived ModioModManagement class that provides a progress status for an operation
* with different labels according to the updates received
**/
UCLASS()
class MODIOUI_API UModioCollectionTileStatus : public UModioModManagementWidgetBase
{
	GENERATED_BODY()
protected:
	int64 PreviousProgressValue = 0;
	FDateTime PreviousUpdateTime;

	void NativeOnSetDataSource() override;
	void SetPercent(float InPercent);
	void UpdateProgress(const struct FModioModProgressInfo& ProgressInfo) override;
	void NativeOnModManagementEvent(FModioModManagementEvent Event) override;

	// These could possibly be moved into the global style
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	FText PendingLabelText;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	FText UpdatePendingLabelText;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	FText UninstallPendingLabelText;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	FText DownloadingLabelText;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	FText ExtractingLabelText;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	FText SubscribedLabelText;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	FText InstalledLabelText;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	FText ErrorLabelText;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	UModioRichTextBlock* StatusText;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	UModioRichTextBlock* StatusPercent;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	UModioProgressBar* ProgressBar;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	UModioSizeBox* ProgressBarSizeBox;
};
