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
#include "ModioCommonCollectionView.generated.h"

class UModioCommonCollectionViewStyle;
class UModioCommonTextBlock;
class UModioCommonModListView;
class UListView;
class UModioCommonButtonBase;
class UModioCommonTabListWidgetBase;

UENUM(BlueprintType)
enum class EModioCommonCollectionViewTabType : uint8
{
	AllInstalled,
	SystemMods
};

/**
 * @brief Collection View that displays a list of downloading, installed or subscribed mods
 * It supports styling through the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI5_API UModioCommonCollectionView
	: public UModioCommonModListBase,
	  public IModioUIModManagementEventReceiver,
	  public IModioUISubscriptionsChangedReceiver,
	  public IModioUIUserChangedReceiver
{
	GENERATED_BODY()

public:
	/**
	 * @brief Sets the style of the Collection View within the Mod.io Common UI styling system
	 * @param InStyle The style to set
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Style")
	void SetStyle(UPARAM(DisplayName = "Style") TSubclassOf<UModioCommonCollectionViewStyle> InStyle);

protected:
	/** @brief The style of the Collection View within the Mod.io Common UI styling system */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), DisplayName = "Style", Category = "Mod.io Common UI")
	TSubclassOf<UModioCommonCollectionViewStyle> ModioStyle;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Collection View|Widgets")
	TObjectPtr<UModioCommonTabListWidgetBase> TabList;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Collection View|Widgets")
	TObjectPtr<UModioCommonTextBlock> DownloadingModsLabelTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Collection View|Widgets")
	TObjectPtr<UModioCommonTextBlock> DownloadingModsDescriptionTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Collection View|Widgets")
	TObjectPtr<UModioCommonTextBlock> NumOfDownloadingModsTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, MustImplement = "ModioCommonModListViewInterface"), Category = "Mod.io Common UI|Collection View|Widgets")
	TObjectPtr<UListView> DownloadingModList;

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

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, MustImplement = "ModioCommonModListViewInterface"), Category = "Mod.io Common UI|Collection View|Widgets")
	TObjectPtr<UListView> InstalledModList;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Collection View|Widgets")
	TObjectPtr<UModioCommonButtonBase> FetchUpdateButton;

protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Collection View|Update")
	void RefreshListByTabId(FName TabId);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Collection View|Update")
	void UpdateDownloadingMods();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Collection View|Update")
	void UpdateInstalledMods();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Collection View|Update")
	bool AreDownloadingSameMods(const TArray<FModioModCollectionEntry>& NewDownloadingMods) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Collection View|Update")
	bool AreInstalledSameMods(const TArray<FModioModCollectionEntry>& NewInstalledMods) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Collection View|Update")
	void OnFetchUpdatesClicked();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Collection View|Update")
	void OnFetchExternalCompleted(FModioErrorCode ErrorCode);

	void UpdateInputBindings();

	//~ Begin UCommonActivatableWidget Implementation
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	//~ End UCommonActivatableWidget Implementation

	//~ Begin UUserWidget Interface
public:
	virtual void SynchronizeProperties() override;
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	//~ End UUserWidget Interface

	virtual void NativeUserChanged(TOptional<FModioUser> NewUser) override;
	virtual void NativeOnModManagementEvent(FModioModManagementEvent Event) override;
	virtual void NativeOnSubscriptionsChanged(FModioModID ModID, bool bNewSubscriptionState) override;
	
protected:
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Mod.io Common UI|Collection View")
	TSet<FModioModID> ModIDsWithErrors;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Mod.io Common UI|Collection View")
	EModioCommonCollectionViewTabType ViewTabType = EModioCommonCollectionViewTabType::AllInstalled;
};