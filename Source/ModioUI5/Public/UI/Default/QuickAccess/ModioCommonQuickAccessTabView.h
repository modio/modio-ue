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
#include "UI/EventHandlers/IModioUIModManagementEventReceiver.h"
#include "UI/EventHandlers/IModioUISubscriptionsChangedReceiver.h"
#include "UI/Foundation/Base/QuickAccess/ModioCommonQuickAccessViewBase.h"
#include "ModioCommonQuickAccessTabView.generated.h"

class UModioCommonBorder;
class UModioCommonTextBlock;
class UModioCommonDynamicImage;
class UModioCommonButtonBase;
class UModioCommonModListView;
class UModioCommonStorageSpaceTrackerWidget;
class UModioCommonStorageSpaceTrackerUserWidget;
class UModioCommonModOperationTrackerWidget;
class UModioCommonProfileImage;
class UModioCommonQuickAccessTabViewStyle;
class UListView;


/**
 * @brief Quick Access View that displays a tabbed view of the user's information, such as their profile image, storage space, and mod management
 * It supports styling through the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI5_API UModioCommonQuickAccessTabView
	: public UModioCommonQuickAccessViewBase,
	  public IModioUIModManagementEventReceiver,
	  public IModioUISubscriptionsChangedReceiver
{
	GENERATED_BODY()

public:
	/**
	 * @brief Sets the style of the Quick Access View within the Mod.io Common UI styling system
	 * @param InStyle The style to set
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Style")
	void SetStyle(UPARAM(DisplayName = "Style") TSubclassOf<UModioCommonQuickAccessTabViewStyle> InStyle);

protected:
	/** @brief The style of the Quick Access View within the Mod.io Common UI styling system */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), DisplayName = "Style", Category = "Mod.io Common UI")
	TSubclassOf<UModioCommonQuickAccessTabViewStyle> ModioStyle;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonBorder> InternalBackgroundBorder;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonBorder> OverlayBackgroundBorder;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonProfileImage> ProfileImage;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonTextBlock> UserNameTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonTextBlock> InstalledModsLabelTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonTextBlock> InstalledModsNumberTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonStorageSpaceTrackerUserWidget> StorageSpaceTrackerUserWidget;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonButtonBase> CloseButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonButtonBase> MyProfileButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonButtonBase> AuthButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonButtonBase> MyCollectionButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonTextBlock> CurrentDownloadsLabelTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonTextBlock> CurrentDownloadsNumberTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, MustImplement = "ModioCommonModListViewInterface"), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UListView> CurrentDownloadsModList;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonTextBlock> InQueueLabelTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonTextBlock> InQueueNumberTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, MustImplement = "ModioCommonModListViewInterface"), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UListView> InQueueModList;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonTextBlock> CompletedLabelTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonTextBlock> CompletedNumberTextBlock;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, MustImplement = "ModioCommonModListViewInterface"), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UListView> CompletedModList;

public:
	UModioCommonQuickAccessTabView();

protected:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void ClearModsViewSelection();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void UpdateInputActions();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void UpdateCurrentDownloadsMods();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void UpdateInQueueMods();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void UpdateCompletedNumber();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void ContainsMod(UListView* ModsView, const FModioModCollectionEntry& ModEntry, bool& bContains, bool& bDifferentState) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	bool AreSameMods(UListView* ModsView, const TArray<FModioModCollectionEntry>& NewDownloadingMods) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void HandleOnCloseButtonClicked();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void HandleOnMyProfileButtonClicked();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void HandleOnAuthButtonClicked();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void HandleOnCollectionButtonClicked();

protected:
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
public:
	virtual void SynchronizeProperties() override;
protected:
	virtual void NativeUserChanged(TOptional<FModioUser> NewUser) override;
	virtual void NativeOnModManagementEvent(FModioModManagementEvent Event) override;
	virtual void NativeOnSubscriptionsChanged(FModioModID ModID, bool bNewSubscriptionState) override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Mod.io Common UI")
	int32 CachedNumOfCurrentlyInstalledMods = 0;
};
