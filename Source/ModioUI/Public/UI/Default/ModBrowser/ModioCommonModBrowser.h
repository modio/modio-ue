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
#include "UI/EventHandlers/IModioUIUserChangedReceiver.h"
#include "UI/Foundation/Base/ModBrowser/ModioCommonModBrowserBase.h"
#include "Core/ModioFilterParamsUI.h"
#include "ModioCommonModBrowser.generated.h"

enum class ECommonInputType : uint8;
class UModioCommonSearchViewBase;
class UModioCommonModBrowserStyle;
class UModioCommonSearchTabView;
class UModioCommonButtonBase;
class UScrollBox;
class UModioCommonWidgetSwitcher;
class UModioCommonTabListWidgetBase;
class UModioCommonUserProfileBase;
class UModioCommonTabButtonBase;
class UModioCommonModListBase;
class UModioCommonActivatableWidgetStack;
class UModioCommonQuickAccessViewBase;
class UModioCommonModDetailsViewBase;
class UModioCommonActionBar;
class UModioCommonDialogInfo;
struct FModioModInfo;

UENUM(BlueprintType)
enum class EModioCommonSearchViewType : uint8
{
	SearchResults,
	Collection
};

/**
 * Modio Common UI widget that displays a mod browser. This is a main entry point for mod browsing, searching, authorisation, etc
 * It supports styling through the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonModBrowser : public UModioCommonModBrowserBase
{
	GENERATED_BODY()

protected:
	/** Notification controller widget that handles displaying notifications to the user */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, MustImplement = "/Script/ModioUICore.ModioUINotificationController"), Category = "Mod.io Common UI")
	TObjectPtr<UWidget> NotificationController;

	/** User Profile widget that displays the basic information about a user */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonUserProfileBase> UserProfile;

	/** Tab list widget that displays the tabs for the mod browser (popular, collection, etc) */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonTabListWidgetBase> TabList;

	/** Content stack that displays the actual content for the mod browser */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonActivatableWidgetStack> ContentStack;

	/** Stack that displays the authentication view */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonActivatableWidgetStack> AuthStack;

	/** Stack that displays the report mod view */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonActivatableWidgetStack> ReportStack;

	/** Stack that displays on the right side of the screen */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonActivatableWidgetStack> RightTabStack;

	/** Stack that displays the dialog views */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonActivatableWidgetStack> DialogStack;

	/** Action bar that displays the currently possible actions for interacting by the user */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonActionBar> ActionBar;

protected:
	//~ Begin UUserWidget Interface
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
public:
	virtual void SynchronizeProperties() override;
	//~ End UUserWidget Interface

protected:
	//~ Begin UModioCommonActivatableWidget Interface
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	//~ End UModioCommonActivatableWidget Interface
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Featured")
	bool ShowFeaturedView();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Featured")
	bool HideFeaturedView();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Collection")
	bool ShowCollectionView();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Collection")
	bool HideCollectionView();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Quick Access")
	bool ShowQuickAccessView();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Quick Access")
	bool HideQuickAccessView();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Search")
	bool ShowSearchView(EModioCommonSearchViewType SearchType, const FModioModCategoryParams& CurrentFilterParams, const FModioModCategoryParams& DefaultFilterParams = FModioModCategoryParams());

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Search")
	bool HideSearchView();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Report")
	bool HideReportMod();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Mod Details")
	bool ShowModDetailsView(const FModioModInfo& ModInfo);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Mod Details")
	bool HideModDetailsView();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void HandleViewChanged();

protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Tab")
	bool AddTab(FName TabNameID, const FText& TabText, TSubclassOf<UModioCommonActivatableWidget> ContentClass);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Tab")
	bool AddEmptyTab(FName TabNameID, const FText& TabText);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Tab")
	bool AddContentForTab(FName TabNameID, TSubclassOf<UModioCommonActivatableWidget> ContentClass, UModioCommonActivatableWidget*& OutContent);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Tab")
	void RemoveTab(FName TabNameID);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Tab")
	void ClearTabs();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Tab")
	bool GetViewFromTabNameID(FName TabNameID, TSubclassOf<UModioCommonActivatableWidget>& OutView) const;

	void OnInputMethodChanged(ECommonInputType NewInputType);

	//~ Begin IModioModBrowserInterface Interface
	virtual void ShowDetailsForMod_Implementation(FModioModID ModID) override;
	virtual void ShowSearchResults_Implementation(const FModioModCategoryParams& FilterParams, bool bIsDefaultFilter) override;
	virtual void ShowUserAuth_Implementation() override;
	virtual void LogOut_Implementation() override;
	virtual void ShowReportMod_Implementation(UObject* DialogDataSource) override;
	virtual bool GetIsCollectionModDisableUIEnabled_Implementation() override;
	virtual void ShowDialog_Implementation(UObject* DialogDataSource) override;
	//~ End IModioModBrowserInterface Interface

	TOptional<FModioModCategoryParams> PendingFilterParams;
	TOptional<bool> PendingIsDefaultFilter;
};
