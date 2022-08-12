// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModioModBrowserParams.h"
#include "UI/BaseWidgets/ModioListView.h"
#include "UI/BaseWidgets/ModioUIAsyncLoader.h"
#include "UI/CommonComponents/ModioWidgetCarousel.h"
#include "UI/Interfaces/IModioUIAsyncOperationWidget.h"
#include "UI/Views/CategoryBrowser/ModioFeaturedCategory.h"
#include "UI/Views/ModioMenuView.h"
#include "UI/Views/ModDetails/ModioAsyncOpWrapperWidget.h"

#include "ModioFeaturedView.generated.h"

/**
 *
 */
UCLASS()
class MODIOUI_API UModioFeaturedView : public UModioMenuView,
									   public IModioUIModInfoReceiver,
									   public IModioUIAsyncOperationWidget
{
	GENERATED_BODY()
protected:
	int32 CurrentlySelectedCategoryColumn = INDEX_NONE;
	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioWidgetCarousel* PrimaryFeaturedCategory;

	/// @brief Widget implementing ModioUIAsyncHandlerWidget to handle swapping between error widget, loading widget, or
	/// the Primary Featured Category
	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidgetOptional, MustImplement = "ModioUIAsyncHandlerWidget"))
	UWidget* PrimaryFeaturedCategoryLoader;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioListView* AdditionalCategories;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widgets", meta = (BindWidget))
	UModioAsyncOpWrapperWidget* FeaturedViewContent;

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
	UFUNCTION()
	int32 GetSelectionIndex();
	UFUNCTION()
	virtual void CategorySelectionChanged(int32 Index, UModioFeaturedCategory* RealCategory);

	virtual void NativeOnListAllModsRequestCompleted(FString RequestIdentifier, FModioErrorCode ec,
													 TOptional<FModioModInfoList> List) override;
	virtual bool ShouldShowSearchButtonForMenu() override;

public:
	void SynchronizeProperties() override;
};
