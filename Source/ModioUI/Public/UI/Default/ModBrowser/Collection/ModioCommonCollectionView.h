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
#include "UI/Foundation/Base/ModBrowser/ModioCommonModListBase.h"
#include "UI/EventHandlers/IModioUIModInfoReceiver.h"
#include "UI/EventHandlers/IModioUIModManagementEventReceiver.h"
#include "UI/EventHandlers/IModioUISubscriptionsChangedReceiver.h"
#include "UI/EventHandlers/IModioUIUserChangedReceiver.h"
#include "Core/ModioFilterParamsUI.h"
#include "ModioCommonCollectionView.generated.h"

class UModioCommonCollectionViewStyle;
class UModioCommonTextBlock;
class UModioCommonModListView;
class UListView;
class UModioCommonButtonBase;
class UModioCommonTabListWidgetBase;

/**
 * Collection View that displays a list of downloading, installed or subscribed mods
 * It supports styling through the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonCollectionView
	: public UModioCommonModListBase,
	  public IModioUIModManagementEventReceiver,
	  public IModioUISubscriptionsChangedReceiver,
	  public IModioUIUserChangedReceiver
{
	GENERATED_BODY()

public:
#if WITH_EDITORONLY_DATA
	/** Whether the list view should show the no results screen, for previewing purposes */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Preview")
	bool bPreviewShowNoResults = false;
#endif

public:
	/**
	 * Sets the style of the Collection View within the Mod.io Common UI styling system
	 * @param InStyle The style to set
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Style")
	void SetStyle(UPARAM(DisplayName = "Style") TSubclassOf<UModioCommonCollectionViewStyle> InStyle);

protected:
	/** The style of the Collection View within the Mod.io Common UI styling system */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), DisplayName = "Style", Category = "Mod.io Common UI")
	TSubclassOf<UModioCommonCollectionViewStyle> ModioStyle;

protected:
	/** The container that holds the no results screen */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UPanelWidget> NoResultsContainer;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Collection View|Widgets")
	TObjectPtr<UModioCommonTextBlock> NumOfDownloadingModsTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Collection View|Widgets")
	TObjectPtr<UModioCommonTextBlock> InstalledModsLabelTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Collection View|Widgets")
	TObjectPtr<UModioCommonTextBlock> InstalledModsDescriptionTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Collection View|Widgets")
	TObjectPtr<UModioCommonTextBlock> NumOfInstalledModsTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Collection View|Widgets")
	TObjectPtr<UModioCommonTextBlock> ErrorsTextBlock;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Collection View|Widgets")
	TObjectPtr<UModioCommonTextBlock> NumOfErrorsTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, MustImplement = "/Script/ModioUI.ModioCommonModListViewInterface"), Category = "Mod.io Common UI|Collection View|Widgets")
	TObjectPtr<UListView> ModList;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Collection View|Widgets")
	TObjectPtr<UModioCommonButtonBase> FetchUpdateButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Collection View|Widgets")
	TObjectPtr<UModioCommonButtonBase> FilterButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonTextBlock> FilterCounterTextBlock;

protected:
	/**
	 * Sets the visibility of the no results screen (the screen that is displayed when there are no mods to display)
	 * @param bVisible Whether the no results screen should be visible or not
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Collection View")
	void SetNoResultsVisibility(bool bVisible);

	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Collection View|Update")
	void SortAToZ(TArray<FModioModCollectionEntry>& ModListEntries);

	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Collection View|Update")
	void SortZToA(TArray<FModioModCollectionEntry>& ModListEntries);

	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Collection View|Update")
	void SortSizeOnDisk(TArray<FModioModCollectionEntry>& ModListEntries);

	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Collection View|Update")
	void SortRecentlyUpdated(TArray<FModioModCollectionEntry>& ModListEntries);

	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Collection View|Update")
	void ApplySortingAndFiltering(TArray<FModioModCollectionEntry>& ModListEntries);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void ShowSearchView();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Collection View|Update")
	void UpdateMods();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Collection View|Update")
	void OnFetchUpdatesClicked();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Collection View|Update")
	void OnFetchExternalCompleted(FModioErrorCode ErrorCode);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Collection View|Update")
	void UpdateInputBindings();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Collection View|Update")
	FModioModCategoryParams GetDefaultCategoryFilterParams(bool bUserAuthenticated);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Collection View|Update")
	void SetDefaultCategoryFilterParams(bool bUserAuthenticated);

	//~ Begin UCommonActivatableWidget Implementation
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	//~ End UCommonActivatableWidget Implementation

	//~ Begin UUserWidget Interface
public:
	virtual void SynchronizeProperties() override;
protected:
	virtual void NativePreConstruct() override;
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	//~ End UUserWidget Interface

	virtual void NativeUserChanged(TOptional<FModioUser> NewUser) override;
	virtual void NativeOnModManagementEvent(FModioModManagementEvent Event) override;
	virtual void NativeOnSubscriptionsChanged(FModioModID ModID, bool bNewSubscriptionState) override;
	
protected:
	/** Mod IDs which had errors during this session */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Mod.io Common UI|Collection View")
	TSet<FModioModID> ModIDsWithErrors;

	/** Last applied filter params */
	FModioModCategoryParams LastAppliedFilterParams;
};
