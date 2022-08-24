// Fill out your copyright notice in the Description page of Project Settings.

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
#include "UI/Interfaces/IModioInputMappingAccessor.h"
#include "UI/Views/CategoryBrowser/ModioFeaturedView.h"
#include "UI/Views/Collection/ModioCollectionView.h"

#include "ModioMenu.generated.h"

UENUM(BlueprintType)
enum class EModioMenuScreen : uint8
{
	EMMS_Featured = 0,
	EMMS_Collection = 1,
	EMMS_SearchResults = 2,
	EMMS_ModDetails = 3
};

UENUM(BlueprintType)
enum class EModioMenuDrawer : uint8
{
	EMMD_RefineSearch = 0,
	EMMD_DownloadQueue = 1
};
/**
 *
 */
UCLASS()
class MODIOUI_API UModioMenu : public UModioUserWidgetBase
{
public:
	void ShowAuthenticationChoiceDialog();
	void SetPageIndex(int Index);
	void ShowModUnsubscribeDialog(UObject* DialogDataSource);
	void ShowLogoutDialog();
	void ShowModReportDialog(UObject* DialogDataSource);
	void ShowReportEmailDialog(UObject* DialogDataSource);
	void ShowUninstallConfirmationDialog(UObject* DialogDataSource);
	int32 GetPageIndex();
	void RefreshDownloadQueue();

private:
	GENERATED_BODY()
	bool bRoutedOnViewChanged = false;

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

	// This will either live here or on the menubar depending on how we want focus to work
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnViewChanged(int64 ViewIndex);

	UPROPERTY(BlueprintReadOnly,Category="Widgets", meta = (BindWidget))
	UNativeWidgetHost* MenuTitleContent;

	UPROPERTY(BlueprintReadOnly,Category="Widgets", meta = (BindWidget))
	UModioWidgetSwitcher* ViewController;

	UPROPERTY(BlueprintReadOnly,Category="Widgets", meta = (BindWidget))
	UModioMenuBar* MenuBar;

	UPROPERTY(BlueprintReadOnly,Category="Widgets", meta = (BindWidget))
	UModioImage* Background;

	UPROPERTY(BlueprintReadOnly,Category="Widgets", meta = (BindWidget))
	UModioDrawerController* DrawerController;

	UPROPERTY(BlueprintReadOnly,Category="Widgets", meta = (BindWidget))
	UModioDialogController* DialogController;

	UPROPERTY(BlueprintReadOnly,Category="Widgets", meta = (BindWidget, MustImplement = "ModioUINotificationController"))
	UWidget* NotificationController;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widgets")
	TSubclassOf<UModioMenuView> FeaturedView;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widgets")
	TSubclassOf<UModioMenuView> CollectionView;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widgets")
	TSubclassOf<UModioMenuView> SearchResultsView;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widgets")
	TSubclassOf<UModioMenuView> ModDetailsView;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widgets")
	TSubclassOf<UModioDrawer> DownloadProgressDrawer;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widgets")
	TSubclassOf<UModioDrawer> RefineSearchDrawer;

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
};
