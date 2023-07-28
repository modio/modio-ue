/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#pragma once

#include "Components/NativeWidgetHost.h"
#include "CoreMinimal.h"
#include "UI/BaseWidgets/ModioDrawerController.h"
#include "UI/BaseWidgets/ModioImage.h"
#include "UI/BaseWidgets/ModioUserWidgetBase.h"
#include "UI/BaseWidgets/ModioWidgetSwitcher.h"
#include "UI/CommonComponents/ModioDrawer.h"
#include "UI/CommonComponents/ModioMenuBar.h"
#include "UI/Dialogs/ModioDialogController.h"
#include "UI/EventHandlers/IModioUIUserChangedReceiver.h"
#include "UI/Interfaces/IModioInputMappingAccessor.h"
#include "UI/Views/CategoryBrowser/ModioFeaturedView.h"
#include "UI/Views/Collection/ModioCollectionView.h"

#include "ModioMenu.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDownloadQueueClosed);
/**
* Enumerator with possible screen menus to display
**/
UENUM(BlueprintType)
enum class EModioMenuScreen : uint8
{
	/** The menu is featured  **/
	EMMS_Featured = 0,

	/** The menu has multiple elements to display **/
	EMMS_Collection = 1,

	/** The menu represents search results **/
	EMMS_SearchResults = 2,

	/** The menu has mod details **/
	EMMS_ModDetails = 3
};

/**
* Enumerator with possible states of a menu drawer
**/
UENUM(BlueprintType)
enum class EModioMenuDrawer : uint8
{
	/** Redefine the search in the menu **/
	EMMD_RefineSearch = 0,

	/** Add an element to the download queue **/
	EMMD_DownloadQueue = 1
};

/**
* Modio UI element that wraps a menu widget, in other words, it stores references to 
* other visual instances like a controller
**/
UCLASS()
class MODIOUI_API UModioMenu : public UModioUserWidgetBase, public IModioUIUserChangedReceiver
{
public:
	void ShowAuthenticationChoiceDialog();
	void SetPageIndex(int Index);
	void ShowModUnsubscribeDialog(UObject* DialogDataSource);
	void ShowLogoutDialog();
	void ShowModReportDialog(UObject* DialogDataSource);
	void ShowReportEmailDialog(UObject* DialogDataSource);
	void ShowUninstallConfirmationDialog(UObject* DialogDataSource);
	void SetShowCursor(bool bShow);
	void CreateHideMouseCursorWidget(TSubclassOf<UUserWidget> widgetClass);
	int32 GetPageIndex();
	void RefreshDownloadQueue();

	FDownloadQueueClosed OnDownloadQueueClosed;

private:
	GENERATED_BODY()
	bool bRoutedOnViewChanged = false;

	bool bMenuFocused = false;

	EMouseCursor::Type CachedBacgroundCursorType;

protected:
	// Action Delegates
	// Can the user change which page is visible?
	bool CanExecutePageChange();
	// Can the user show the refine search drawer?
	bool CanShowRefineSearch();

	// Can the user show the download queue drawer?
	bool CanShowDownloadQueue();

	// Swaps between Browse and Collection views
	void ChangePagePrevious();
	void ChangePageNext();

	/// @brief Toggles the display of the refine search drawer
	void ToggleRefineSearchDrawer();

	/// @brief Toggles the display of the download queue drawer
	void ToggleDownloadQueueDrawer();

	bool CanGoToPreviousSubmenu();
	void GoToPreviousSubmenu();
	// End Action Delegates

	virtual void BuildCommandList(TSharedRef<FUICommandList> CommandList) override;

	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	virtual void NativeOnInitialized() override;

	void UpdateRefineSearchDrawerSettings(FModioFilterParams Settings);

	UFUNCTION()
	void OnSearchSettingsChanged(FModioFilterParams Settings);

	UFUNCTION()
	void HandleViewChanged(int32 ViewIndex);

	virtual void NativeOnViewChanged(int64 ViewIndex);
	virtual void NativeTick(const FGeometry& InGeometry, float DeltaTime) override;

	// Using switched from NativeOnKeyDown to NativeOnPreviewKeyDown to capture input event when the menu is hidden 
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnViewChanged(int64 ViewIndex);

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Widgets", meta = (BindWidgetAnimOptional))
	UWidgetAnimation* ViewChangedAnim;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Widgets", meta = (BindWidgetAnimOptional))
	UWidgetAnimation* DialogAnim;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget))
	UModioWidgetSwitcher* ViewController;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget))
	UModioMenuBar* MenuBar;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget))
	UModioImage* Background;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget))
	UModioDrawerController* DrawerController;
	
	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget))
	UModioDialogController* DialogController;

	UPROPERTY(BlueprintReadOnly, Category="Widgets")
	UUserWidget* HideCursorWidget;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets",
			  meta = (BindWidget, MustImplement = "ModioUINotificationController"))
	UWidget* NotificationController;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget))
	class UUserWidget* ModioMenuBarWidget;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	TSubclassOf<UModioMenuView> FeaturedView;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	TSubclassOf<UModioMenuView> CollectionView;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	TSubclassOf<UModioMenuView> SearchResultsView;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	TSubclassOf<UModioMenuView> ModDetailsView;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	TSubclassOf<UModioDrawer> DownloadProgressDrawer;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	TSubclassOf<UModioDrawer> RefineSearchDrawer;

	UUserWidget* ActiveViewWidget;

	virtual void NativeUserChanged(TOptional<FModioUser> NewUser) override;

public:
	void ShowDetailsForMod(FModioModID ID);
	bool RequestShowSearchResults(FModioFilterParams Params);
	void ForceCloseDownloadDrawer()
	{
		DrawerController->SetDrawerExpanded((int32) EModioMenuDrawer::EMMD_DownloadQueue, false, false);
	}
	void ForceCloseSearchDrawer()
	{
		DrawerController->SetDrawerExpanded((int32) EModioMenuDrawer::EMMD_RefineSearch, false, false);
	}

	UModioDialogController* GetDialogController();
	UModioDrawerController* GetDrawerController();
};
