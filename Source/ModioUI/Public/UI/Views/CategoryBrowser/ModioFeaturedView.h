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

#include "CoreMinimal.h"
#include "ModioModBrowserParams.h"
#include "UI/BaseWidgets/ModioListView.h"
#include "UI/BaseWidgets/ModioUIAsyncLoader.h"
#include "UI/CommonComponents/ModioWidgetCarousel.h"
#include "UI/EventHandlers/IModioUIUserChangedReceiver.h"
#include "UI/Interfaces/IModioUIAsyncOperationWidget.h"
#include "UI/Views/CategoryBrowser/ModioFeaturedCategory.h"
#include "UI/Views/ModDetails/ModioAsyncOpWrapperWidget.h"
#include "UI/Views/ModioMenuView.h"

#include "ModioFeaturedView.generated.h"

/**
* Class to create a menu view for a mod feature
**/
UCLASS()
class MODIOUI_API UModioFeaturedView : public UModioMenuView,
									   public IModioUIModInfoReceiver,
									   public IModioUIAsyncOperationWidget,
									   public IModioUIUserChangedReceiver
{
	GENERATED_BODY()
protected:
	bool bModsFound = false;
	int32 CurrentlySelectedCategoryColumn = INDEX_NONE;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	UModioWidgetCarousel* PrimaryFeaturedCategory;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget))
	class UModioErrorRetryWidget* ModioErrorWithRetryWidget;

	/// @brief Widget implementing ModioUIAsyncHandlerWidget to handle swapping between error widget, loading widget, or
	/// the Primary Featured Category
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets",
			  meta = (BindWidgetOptional, MustImplement = "ModioUIAsyncHandlerWidget"))
	UWidget* PrimaryFeaturedCategoryLoader;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	UModioListView* AdditionalCategories;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	UModioAsyncOpWrapperWidget* FeaturedViewContent;

	UPROPERTY(meta = (BindWidget))
	class UModioGridPanel* PrimaryCategoryGridPanel;

	// Holds the featured items so that they don't go out of scope
	UPROPERTY()
	TArray<UObject*> CachedFeaturedItems;

	void NativeBeginLoadExternalData();

	void OnAdditionalCategoryCreated(UUserWidget& CategoryWidget);

	virtual void NativeConstruct() override;

	virtual void NativeOnInitialized() override;
	virtual void NativeRequestOperationRetry() override;
	void FetchPrimaryCategoryMods();

	virtual FNavigationReply NativeOnNavigation(const FGeometry& InGeometry,
												const FNavigationEvent& InNavigationEvent) override;

	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	UFUNCTION(BlueprintCallable, Category = "Widgets")
	void SetFocusToPrimaryCategory();

	UFUNCTION(BlueprintCallable, Category = "Widgets")
	void SetFocusToAdditionalCategory();

	UFUNCTION()
	int32 GetSelectionIndex();
	UFUNCTION()
	virtual void CategorySelectionChanged(int32 Index, UModioFeaturedCategory* RealCategory);

	virtual void NativeOnListAllModsRequestCompleted(FString RequestIdentifier, FModioErrorCode ec,
													 TOptional<FModioModInfoList> List) override;
	virtual bool ShouldShowSearchButtonForMenu() override;

	virtual void NativeUserChanged(TOptional<FModioUser> NewUser) override;

public:
	/**
	*  Called by the editor to update modified state
	**/
	void SynchronizeProperties() override;
};
