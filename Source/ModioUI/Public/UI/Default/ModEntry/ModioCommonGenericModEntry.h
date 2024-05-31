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
#include "UI/Foundation/Base/ModBrowser/ModioCommonModEntryBase.h"
#include "ModioCommonGenericModEntry.generated.h"

class UModioCommonModOperationTrackerWidget;
class UModioCommonModOperationTrackerUserWidget;
class UModioCommonBorder;
class UModioCommonProgressBar;
class UModioCommonGenericModEntryStyle;
class UModioCommonDynamicImage;
class UModioCommonRichTextBlock;
class UModioCommonTextBlock;
class UModioCommonButtonBase;

/**
 * Generic Mod Entry that can be used to display mod information and perform mod operations
 * It supports styling through the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonGenericModEntry : public UModioCommonModEntryBase
{
	GENERATED_BODY()

public:

#if WITH_EDITORONLY_DATA
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Mod.io Common UI|Preview")
	FModioModInfo PreviewModInfo;
#endif

	/**
	 * Sets the style of the Generic Mod Entry within the Mod.io Common UI styling system
	 * @param InStyle The style to set
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Style")
	void SetStyle(UPARAM(DisplayName = "Style") TSubclassOf<UModioCommonGenericModEntryStyle> InStyle);

protected:
	/** The style of the Generic Mod Entry within the Mod.io Common UI styling system */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), DisplayName = "Style", Category = "Mod.io Common UI")
	TSubclassOf<UModioCommonGenericModEntryStyle> ModioStyle;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets|Background")
	TObjectPtr<UModioCommonBorder> Background;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets|Foreground")
	TObjectPtr<UPanelWidget> ForegroundContainer;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets|Image")
	TObjectPtr<UModioCommonDynamicImage> ModImage;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets|Text")
	TObjectPtr<UModioCommonRichTextBlock> ModNameTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets|Text|Operation")
	TObjectPtr<UModioCommonRichTextBlock> OperationLabelTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets|Text|Operation")
	TObjectPtr<UModioCommonRichTextBlock> OperationDetailsTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets|Text|Speed")
	TObjectPtr<UModioCommonRichTextBlock> SpeedLabelTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets|Text|Speed")
	TObjectPtr<UModioCommonRichTextBlock> SpeedDetailsTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets|Text|Downloads")
	TObjectPtr<UModioCommonRichTextBlock> DownloadsLabelTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets|Text|Downloads")
	TObjectPtr<UModioCommonRichTextBlock> DownloadsDetailsTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets|Text|Size")
	TObjectPtr<UModioCommonRichTextBlock> SizeLabelTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets|Text|Size")
	TObjectPtr<UModioCommonRichTextBlock> SizeDetailsTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets|Text|Rating")
	TObjectPtr<UModioCommonRichTextBlock> RatingPercentageTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets|Text|Rating")
	TObjectPtr<UModioCommonRichTextBlock> RatingPositiveTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets|Text|Rating")
	TObjectPtr<UModioCommonRichTextBlock> RatingNegativeTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets|Buttons")
	TObjectPtr<UModioCommonButtonBase> SubscribeButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets|Buttons")
	TObjectPtr<UModioCommonButtonBase> CancelButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets|Buttons")
	TObjectPtr<UModioCommonButtonBase> EnableButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets|Buttons")
	TObjectPtr<UModioCommonButtonBase> DisableButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets|Buttons")
	TObjectPtr<UModioCommonButtonBase> OpenModDetailsButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets|Buttons")
	TObjectPtr<UModioCommonButtonBase> ForceUninstallButton;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets|Buttons")
	TObjectPtr<UModioCommonButtonBase> PrioritizeTransferButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets|Buttons")
	TObjectPtr<UModioCommonRichTextBlock> PercentProgressTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets|Buttons")
	TObjectPtr<UModioCommonProgressBar> PercentProgressBar;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ClampMin = 0), Category = "Properties")
	int32 MinDecimalsFileSize = 0;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ClampMin = 0), Category = "Properties")
	int32 MaxDecimalsFileSize = 1;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ClampMin = 0), Category = "Properties")
	int32 MinDecimalsProgress = 1;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ClampMin = 0), Category = "Properties")
	int32 MaxDecimalsProgress = 1;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ClampMin = 0), Category = "Properties")
	int32 MinDecimalsSpeed = 1;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ClampMin = 0), Category = "Properties")
	int32 MaxDecimalsSpeed = 1;

protected:
	//~ Begin UWidget Interface
	virtual bool Initialize() override;
	virtual void NativePreConstruct() override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
public:
	virtual void SynchronizeProperties() override;
protected:
	//~ End UWidget Interface

	//~ Begin UModioCommonModEntryBase Interface
	virtual void NativeUpdateStyling(bool bIsListItemSelected) override;
	//~ End UModioCommonModEntryBase Interface

	//~ Begin UModioCommonActivatableWidget Interface
	virtual void NativeOnSetDataSource() override;
	//~ End UModioCommonActivatableWidget Interface

	//~ Begin IModioUIMediaDownloadCompletedReceiver Interface
	virtual void NativeOnModLogoDownloadCompleted(FModioModID ModID, FModioErrorCode ErrorCode, TOptional<FModioImageWrapper> Image, EModioLogoSize LogoSize) override;
	//~ End IModioUIMediaDownloadCompletedReceiver Interface

	/**
	 * Called when the rating submission is complete
	 * @param ErrorCode The error code
	 * @param Rating The rating that was submitted
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Rating")
	void OnRatingSubmissionComplete(FModioErrorCode ErrorCode, EModioRating Rating);

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

	/**
	 * Switch the visibility of the speed label and details text blocks
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Visibility")
	void SwitchSpeedVisibility(bool bIsVisible);

	/**
	 * Switch the visibility of the cancel button
	 * @param bIsVisible Whether the cancel button should be visible
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Visibility")
	void SwitchCancelButtonVisibility(bool bIsVisible);

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
	 * Switch the visibility of the enable button
	 * @param bIsVisible Whether the enable button should be visible
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Visibility")
	void SwitchForceUninstallButtonVisibility(bool bIsVisible);

protected:
	/**
	 * Called when the user clicks the subscribe button
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Handle")
	void HandleSubscribeClicked();

	/**
	 * Called when the user clicks the enable/disable button
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Handle")
	void HandleSwitchEnabledClicked();

	/**
	 * Called when the user clicks the cancel button
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Handle")
	void HandleCancelClicked();

	/**
	 * Called when the user clicks the open mod details button
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Handle")
	void HandleOpenModDetailsClicked();

	/**
	 * Called when the user clicks the force uninstall button
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Handle")
	void HandleForceUninstallClicked();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Handle")
	void HandlePrioritizeTransferClicked();

private:
	/** Whether the mod image has been obtained or not yet */
	bool bObtainedModImage = false;

	/** Deselection timer used to reduce flickering between focus changing */
	FTimerHandle DeselectTimerHandle;
};