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
 * @brief Generic Mod Entry that can be used to display mod information and perform mod operations
 * It supports styling through the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI5_API UModioCommonGenericModEntry : public UModioCommonModEntryBase
{
	GENERATED_BODY()

public:

#if WITH_EDITORONLY_DATA
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Mod.io Common UI|Preview")
	FModioModInfo PreviewModInfo;
#endif

	/**
	 * @brief Sets the style of the Generic Mod Entry within the Mod.io Common UI styling system
	 * @param InStyle The style to set
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Style")
	void SetStyle(UPARAM(DisplayName = "Style") TSubclassOf<UModioCommonGenericModEntryStyle> InStyle);

protected:
	/** @brief The style of the Generic Mod Entry within the Mod.io Common UI styling system */
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
	
	virtual void NativeOnSetDataSource() override;
	virtual void NativeOnModLogoDownloadCompleted(FModioModID ModID, FModioErrorCode ErrorCode, TOptional<FModioImageWrapper> Image, EModioLogoSize LogoSize) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Rating")
	void OnRatingSubmissionComplete(FModioErrorCode ErrorCode, EModioRating Rating);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Update")
	void UpdateOperationProgressPercent(float InPercent);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Update")
	void UpdateOperationProgressBytes(FModioUnsigned64 Current, FModioUnsigned64 Total);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Update")
	void UpdateSpeed(FModioUnsigned64 DeltaBytes, double DeltaTime);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Visibility")
	void SwitchSpeedVisibility(bool bIsVisible);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Visibility")
	void SwitchCancelButtonVisibility(bool bIsVisible);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Visibility")
	void SwitchEnableButtonVisibility(bool bIsVisible);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Visibility")
	void SwitchDisableButtonVisibility(bool bIsVisible);

	/**
	 * Called to switch the visibility of the enable button
	 *
	 * @param bIsVisible Whether the enable button should be visible
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Visibility")
	void SwitchForceUninstallButtonVisibility(bool bIsVisible);

protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Handle")
	void HandleSubscribeClicked();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Handle")
	void HandleSwitchEnabledClicked();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Handle")
	void HandleCancelClicked();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Handle")
	void HandleOpenModDetailsClicked();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Handle")
	void HandleForceUninstallClicked();

private:
	bool bObtainedModImage = false;

	/** Timer used to reduce flickering between focus changing */
	FTimerHandle DelesectTimerHandle;
};