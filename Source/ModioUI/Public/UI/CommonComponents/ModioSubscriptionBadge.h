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
#include "UI/BaseWidgets/ModioProgressBar.h"
#include "UI/BaseWidgets/ModioRichTextBlock.h"
#include "UI/BaseWidgets/ModioTextBlock.h"
#include "UI/BaseWidgets/ModioUserWidgetBase.h"
#include "UI/CommonComponents/ModioModManagementWidgetBase.h"
#include "UI/EventHandlers/IModioUIModManagementEventReceiver.h"
#include "UI/EventHandlers/IModioUISubscriptionsChangedReceiver.h"

#include "ModioSubscriptionBadge.generated.h"

/**
* Class definition for a Modio subscription badge which contains a progress bar, 
* a badge style, and multiple labels
**/
UCLASS()
class MODIOUI_API UModioSubscriptionBadge : public UModioModManagementWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void NativeOnSetDataSource() override;
	virtual void NativeOnModManagementEvent(FModioModManagementEvent Event) override;

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	void SynchronizeProperties() override;
	const FModioRichTextStyle& GetLabelStyle() const;
	TSharedRef<SWidget> RebuildWidget() override;

	TSharedPtr<class SRetainerWidget> WrappedContent = nullptr;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	UModioProgressBar* ProgressBar;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	UModioRichTextBlock* Label;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (StyleClass = "ModioSubscriptionBadgeStyle"),
			  Category = "Widgets")
	FModioUIStyleRef BadgeStyle;

	// These could possibly be moved into the global style
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	FText PendingLabelText;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	FText DownloadingLabelText;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	FText ExtractingLabelText;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	FText SubscribedLabelText;

	void UpdateProgress(const struct FModioModProgressInfo& ProgressInfo) override;

public:
	/**
	* Update the percentage in the progress bar
	* @param InPercent Updated float value to store in the progress bar
	**/
	void SetPercent(float InPercent);
};
