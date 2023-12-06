/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "CoreMinimal.h"
#include "UI/Foundation/Base/ModDetails/ModioCommonModDetailsViewBase.h"
#include <Components/VerticalBox.h>
#include "UI/Foundation/Components/CheckBox/ModioCommonCheckBox.h"
#include <Components/Button.h>
#include "UI/Foundation/Components/ScrollBox/ModioCommonScrollBox.h"
#include "UI/Foundation/Utilities/ModioCommonTickableWidget.h"

#include "ModioCommonModDetailsView.generated.h"

class UModioCommonButtonBase;
class UModioCommonTextBlock;
class UModioCommonImageGalleryView;
class UModioCommonModDetailsViewStyle;
class UModioCommonModGalleryView;
class UModioCommonModTagList;
class UModioCommonProgressBar;
class UHorizontalBox;
class UModioCommonModOperationTrackerUserWidget;
class UModioCommonModOperationTrackerWidget;

/**
 * @brief Mod Details View widget that displays the details of a mod
 * It supports styling through the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI5_API UModioCommonModDetailsView : public UModioCommonModDetailsViewBase
{
	GENERATED_BODY()

public:

	UModioCommonModDetailsView();

	/**
	 * @brief Sets the style of the Mod Details View within the Mod.io Common UI styling system
	 * @param InStyle The style to set
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Style")
	void SetStyle(UPARAM(DisplayName = "Style") TSubclassOf<UModioCommonModDetailsViewStyle> InStyle);

protected:
	/** @brief The style of the Mod Details View within the Mod.io Common UI styling system */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), DisplayName = "Style", Category = "Mod.io Common UI")
	TSubclassOf<UModioCommonModDetailsViewStyle> ModioStyle;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonTextBlock> ModNameTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonButtonBase> SubscribeButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonButtonBase> CancelButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonButtonBase> RateUpButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonButtonBase> RateDownButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonButtonBase> ReportButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UButton> CollectionButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonTextBlock> ModSummaryTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonTextBlock> ModFullDescriptionLabelTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonTextBlock> ModFullDescriptionTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UHorizontalBox> OperationContainer;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UVerticalBox> ProgressBarContainer;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UHorizontalBox> StatusContainer;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonCheckBox> InstalledCheckBox;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonCheckBox> EnabledCheckBox;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonTextBlock> OperationProgressLabelTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonTextBlock> OperationProgressTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonProgressBar> OperationProgressBar;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonTextBlock> FileSizeLabelTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonTextBlock> FileSizeTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonTextBlock> LastUpdatedLabelTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonTextBlock> LastUpdatedTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonTextBlock> ReleaseDateLabelTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonTextBlock> ReleaseDateTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonTextBlock> SubscribersLabelTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonTextBlock> SubscribersTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonTextBlock> CreatedByLabelTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonTextBlock> CreatedByTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonTextBlock> TagsLabelTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonModTagList> TagsWidget;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonModGalleryView> ModGalleryView;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonScrollBox> ModioCommonDescriptionScrollBox;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonTextBlock> SpeedDetailsTextBlock;

#if WITH_EDITORONLY_DATA
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Properties")
	FModioModInfo PreviewModInfo;
#endif

protected:
	virtual void NativePreConstruct() override;
	virtual bool Initialize() override;
	virtual void NativeOnSetDataSource() override;

public:
	virtual void SynchronizeProperties() override;

protected:
	virtual void NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnInitialized() override;
	virtual void NativeOnSubscriptionsChanged(FModioModID ModID, bool bNewSubscriptionState) override;
	virtual void NativeOnModManagementEvent(FModioModManagementEvent Event) override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;

	/** Callback when the submission of a rating is completed */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Rating")
	void OnRatingSubmissionComplete(FModioErrorCode ErrorCode, EModioRating SubmittedRating);

	/** Activates Subscription/Cancel button */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void ActivateTopButtonsInputBindings();

	/** Activates RateUp, RateDown and Report buttons */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void ActivateBottomButtonsInputBindings();

	/** Activates the tags input bindings */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void ActivateTagsInputBindings();

	/** Activates input actions */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void UpdateInputActions();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Update")
	void UpdateOperationProgressPercent(float InPercent);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Update")
	void UpdateOperationProgressBytes(FModioUnsigned64 Current, FModioUnsigned64 Total);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Update")
	void UpdateSpeed(FModioUnsigned64 DeltaBytes, double DeltaTime);

	/** Shows the mod status information */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Update")
	void ShowStatus();

	/** Shows the progress bar information */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Update")
	void ShowProgress();

	/** Hides the progress bar information */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Update")
	void HideProgress();

protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Handle")
	void HandleSubscribeClicked();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Handle")
	void HandleSwitchEnabledClicked();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Handle")
	void HandleCancelClicked();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Handle")
	void HandleRateUpClicked();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Handle")
	void HandleRateDownClicked();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Handle")
	void HandleReportClicked();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Handle")
	void HandleCollectionClicked();

public:
	/** Timer to put a delay between subscribe and unsubscribe */
	FTimerHandle SubscriptionDelayTimer;

	/** The mod details that was shown recently */
	FModioModID LastModID;

	/** Enables Subscribe button */
	void AllowSubscription(bool allow);

	/** Enables Rate Up button */
	void AllowRatingUp(bool allow);

	/** Enables Rate Down button */
	void AllowRatingDown(bool allow);

	/** The mod details that was shown recently */
	bool IsRateLimited(FModioErrorCode ErrorCode);
};
