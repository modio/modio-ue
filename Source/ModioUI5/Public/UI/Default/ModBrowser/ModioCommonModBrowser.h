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
#include "ModioCommonModBrowser.generated.h"

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
class UCommonActivatableWidgetStack;
class UModioCommonQuickAccessViewBase;
class UModioCommonModDetailsViewBase;
class UModioCommonActionBar;
class UModioCommonDialogInfo;
struct FModioModInfo;

/**
 * @brief Modio Common UI widget that displays a mod browser. This is a main entry point for mod browsing, searching, authorisation, etc
 * It supports styling through the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI5_API UModioCommonModBrowser : public UModioCommonModBrowserBase
{
	GENERATED_BODY()

public:
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Transient, Category = "Mod.io Common UI|Preview")
	bool bPreviewDisplayFeaturedOrCollection = true;
#endif
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, MustImplement = "ModioUINotificationController"), Category = "Mod.io Common UI")
	TObjectPtr<UWidget> NotificationController;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonUserProfileBase> UserProfile;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonTabListWidgetBase> TabList;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonWidgetSwitcher> ContentSwitcher;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI")
	TObjectPtr<UCommonActivatableWidgetStack> AuthStack;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI")
	TObjectPtr<UCommonActivatableWidgetStack> ReportStack;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI")
	TObjectPtr<UCommonActivatableWidgetStack> RightTabStack;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI")
	TObjectPtr<UCommonActivatableWidgetStack> DialogStack;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonActionBar> ActionBar;

protected:
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonModListBase> FeaturedView;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonModListBase> CollectionView;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonModListBase> SearchResultsView;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonModDetailsViewBase> ModDetailsView;

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
	bool ShowSearchView();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Search")
	bool HideSearchView();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Report")
	bool HideReportMod();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Mod Details")
	bool ShowModDetailsView(const FModioModInfo& ModInfo);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Mod Details")
	bool HideModDetailsView();

protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Tab")
	bool AddTab(FName TabNameID, const FText& TabText, TSubclassOf<UModioCommonActivatableWidget> ContentClass, UModioCommonActivatableWidget*& OutContent);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Tab")
	void RemoveTab(FName TabNameID);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Tab")
	void ClearTabs();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Tab")
	bool GetViewFromTabNameID(FName TabNameID, UModioCommonActivatableWidget*& OutView) const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mod.io Common UI|Tab")
	bool bShowSearchViewOnSearchResults = true;

	//~ Begin IModioModBrowserInterface Interface
	virtual void ShowDetailsForMod_Implementation(FModioModID ModID) override;
	virtual void ShowSearchResults_Implementation(const FModioFilterParams& FilterParams) override;
	virtual void ShowUserAuth_Implementation() override;
	virtual void LogOut_Implementation() override;
	virtual void ShowReportMod_Implementation(UObject* DialogDataSource) override;
	virtual bool GetIsCollectionModDisableUIEnabled_Implementation() override;
public:
	void ShowDialog(UModioCommonDialogInfo* DialogInfo);
	//~ End IModioModBrowserInterface Interface
};
