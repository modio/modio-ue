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
#include "Components/VerticalBox.h"
#include "UI/Foundation/Components/CheckBox/ModioCommonCheckBox.h"
#include "Components/Button.h"
#include "UI/Foundation/Components/ScrollBox/ModioCommonScrollBox.h"
#include "UI/Foundation/Utilities/ModioCommonTickableWidget.h"
#include "UI/Interfaces/IModioUIAsyncOperationWidget.h"

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
class UModioCommonErrorWithRetryWidget;

/**
 * Mod Details View widget that displays the details of a mod
 * It supports styling through the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonModDetailsView
	: public UModioCommonModDetailsViewBase,
	  public IModioUIAsyncOperationWidget
{
	GENERATED_BODY()

public:

	UModioCommonModDetailsView();

	/**
	 * Sets the style of the Mod Details View within the Mod.io Common UI styling system
	 * @param InStyle The style to set
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Style")
	void SetStyle(UPARAM(DisplayName = "Style") TSubclassOf<UModioCommonModDetailsViewStyle> InStyle);

protected:
	/** The style of the Mod Details View within the Mod.io Common UI styling system */
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

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets|Buttons")
	TObjectPtr<UModioCommonButtonBase> EnableButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets|Buttons")
	TObjectPtr<UModioCommonButtonBase> DisableButton;

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

	/** Loader widget to display while the mod details are loading */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, MustImplement = "/Script/ModioUICore.ModioUIAsyncHandlerWidget"), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UWidget> ModDetailsLoader;

	/** Widget to display when there is an error loading the mod details */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonErrorWithRetryWidget> ErrorWithRetryWidget;

#if WITH_EDITORONLY_DATA
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Properties")
	FModioModInfo PreviewModInfo;
#endif

protected:
	//~ Begin UUserWidget Interface
	virtual void NativePreConstruct() override;
	virtual bool Initialize() override;
	virtual void NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnInitialized() override;
	//~ End UUserWidget Interface
	
	//~ Begin UModioCommonActivatableWidget Interface
	virtual void NativeOnSetDataSource() override;
	//~ End UModioCommonActivatableWidget Interface

public:
	//~ Begin UWidget Interface
	virtual void SynchronizeProperties() override;
	//~ End UWidget Interface

protected:
	//~ Begin IModioUIModEnableWidget Interface
	virtual void NativeOnModEnabledStateChanged(FModioModID ModID, bool bNewSubscriptionState) override;
	//~ End IModioUIModEnableWidget Interface

	//~ Begin IModioUISubscriptionsChangedReceiver Interface
	virtual void NativeOnSubscriptionsChanged(FModioModID ModID, bool bNewSubscriptionState) override;
	//~ End IModioUISubscriptionsChangedReceiver Interface

	//~ Begin IModioUIModManagementEventReceiver Interface
	virtual void NativeOnModManagementEvent(FModioModManagementEvent Event) override;
	//~ End IModioUIModManagementEventReceiver Interface

	//~ Begin UCommonActivatableWidget Interface
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	//~ End UCommonActivatableWidget Interface

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

	/**
	 * Switch the visibility of the enable button
	 * @param bIsVisible Whether the enable button should be visible
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Visibility")
	void SwitchEnableButtonVisibility(bool bIsVisible);

	/**
	 * Switch the visibility of the disable button
	 * @param bIsVisible Whether the disable button should be visible
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Visibility")
	void SwitchDisableButtonVisibility(bool bIsVisible);

	/**
	 * Updates the operation progress percent (0.0 - 1.0)
	 * @param InPercent The percent to set
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Update")
	void UpdateOperationProgressPercent(float InPercent);

	/**
	 * Updates the number of bytes downloaded and the total number of bytes to download
	 * @param Current The number of bytes downloaded
	 * @param Total The total number of bytes to download
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Update")
	void UpdateOperationProgressBytes(FModioUnsigned64 Current, FModioUnsigned64 Total);

	/**
	 * Updates the download speed
	 * @param DeltaBytes The number of bytes downloaded since the last update
	 * @param DeltaTime The time elapsed since the last update
	 */
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
	/** Called when the user clicks the subscribe button */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Handle")
	void HandleSubscribeClicked();

	/** Called when the user clicks the enable/disable button */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Handle")
	void HandleSwitchEnabledClicked();

	/** Called when the user clicks the cancel button */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Handle")
	void HandleCancelClicked();

	/** Called when the user clicks the rate up button */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Handle")
	void HandleRateUpClicked();

	/** Called when the user clicks the rate down button */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Handle")
	void HandleRateDownClicked();

	/** Called when the user clicks the report button */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Handle")
	void HandleReportClicked();

	/** Called when the user clicks the collection button */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Handle")
	void HandleCollectionClicked();

public:
	/** Timer to put a delay between subscribe and unsubscribe */
	FTimerHandle SubscriptionDelayTimer;

	/** Enables Rate Up button */
	void AllowRatingUp(bool bAllow);

	/** Enables Rate Down button */
	void AllowRatingDown(bool bAllow);

	/** The mod details that was shown recently */
	bool IsRateLimited(FModioErrorCode ErrorCode);
};
