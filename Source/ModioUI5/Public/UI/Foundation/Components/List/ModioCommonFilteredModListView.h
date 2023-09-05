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
#include "UI/EventHandlers/IModioUIModInfoReceiver.h"
#include "UI/Foundation/Base/ModioCommonActivatableWidget.h"
#include "UI/Foundation/Components/List/ModioCommonModListViewInterface.h"
#include "ModioCommonFilteredModListView.generated.h"

class UModioCommonTextBlock;
class UModioCommonButtonBase;
class UListView;
class UModioCommonFilteredModListViewStyle;

/**
 * @brief Filtered Mod List View displays a list of mods that can be filtered
 * It supports styling through the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI5_API UModioCommonFilteredModListView
	: public UModioCommonActivatableWidget,
	  public IModioCommonModListViewInterface,
	  public IModioUIModInfoReceiver
{
	GENERATED_BODY()

public:
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (UIMin = 0, ClampMin = 0), Category = "Mod.io Common UI|Preview")
	int64 PreviewCurrentPageIndex = 2;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (UIMin = 0, ClampMin = 0), Category = "Mod.io Common UI|Preview")
	int64 PreviewTotalMods = 256;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (UIMin = 0, ClampMin = 0), Category = "Mod.io Common UI|Preview")
	int64 PreviewPageSize = 100;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Preview")
	bool bPreviewShowNoResults = false;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Preview")
	bool bPreviewShowLoading = false;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Preview")
	bool bPreviewShowPageNavigation = true;
#endif

public:
	/**
	 * @brief Sets the style of the Filtered Mod List View within the Mod.io Common UI styling system
	 * @param InStyle The style to set
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Style")
	void SetStyle(UPARAM(DisplayName = "Style") TSubclassOf<UModioCommonFilteredModListViewStyle> InStyle);

protected:
	/** @brief The style of the Filtered Mod List View within the Mod.io Common UI styling system */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), DisplayName = "Style", Category = "Mod.io Common UI")
	TSubclassOf<UModioCommonFilteredModListViewStyle> ModioStyle;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, MustImplement = "ModioCommonModListViewInterface"), Category = "Mod.io Common UI")
	TObjectPtr<UListView> ModList;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI")
	TObjectPtr<UPanelWidget> LoadingContainer;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI")
	TObjectPtr<UPanelWidget> NoResultsContainer;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI")
	TObjectPtr<UPanelWidget> PageNavigationContainer;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonButtonBase> PreviousPageButton;
		
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonButtonBase> NextPageButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonTextBlock> CurrentPageTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonTextBlock> TotalPagesTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonTextBlock> TotalModsTextBlock;

protected:
	//~ Begin UModioCommonActivatableWidget Interface
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	virtual void NativeOnSetDataSource() override;
	virtual FString GetRequestIdentifier_Implementation() override;
	//~ End UModioCommonActivatableWidget Interface

	//~ Begin UUserWidget Interface
	virtual void NativePreConstruct() override;
	virtual void NativeOnInitialized() override;
public:
	virtual void SynchronizeProperties() override;
	//~ End UUserWidget Interface

protected:
	//~ Begin IModioCommonModListViewInterface Interface
	virtual void SetFocusOnceListIsPopulated_Implementation(bool bFocus) override;
	virtual void RequestFullClearSelection_Implementation() override;
	virtual void NativeSetListItems(const TArray<UObject*>& InListItems, bool bAddToExisting) override;
	//~ End IModioCommonModListViewInterface Interface

	//~ Begin IModioUIModInfoReceiver Interface
	virtual void NativeOnListAllModsRequestCompleted(FString RequestIdentifier, FModioErrorCode ErrorCode, TOptional<FModioModInfoList> List) override;
	//~ End IModioUIModInfoReceiver Interface

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void UpdateInputActions();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void SetPageNavigationVisibility(bool bVisible);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void HandlePreviousPageClicked();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void HandleNextPageClicked();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void SetNoResultsVisibility(bool bVisible);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void SetLoadingVisibility(bool bVisible);

protected:
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Mod.io Common UI|Properties")
	int64 CurrentPageIndex = 0;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Mod.io Common UI|Properties")
	int64 TotalMods = 0;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Mod.io Common UI|Properties")
	int64 PageSize = 0;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Mod.io Common UI|Properties")
	int64 TotalPages = 0;
};
