// Fill out your copyright notice in the Description page of Project Settings.

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
 * 
 */
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

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioProgressBar* ProgressBar;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioRichTextBlock* Label;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (StyleClass = "ModioSubscriptionBadgeStyle"),Category="Widgets")
	FModioUIStyleRef BadgeStyle;

	// These could possibly be moved into the global style
	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets")
	FText PendingLabelText;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets")
	FText DownloadingLabelText;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets")
	FText ExtractingLabelText;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets")
	FText SubscribedLabelText;

	void UpdateProgress(const struct FModioModProgressInfo& ProgressInfo) override;

public:
	void SetPercent(float InPercent);
};
