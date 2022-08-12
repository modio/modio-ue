// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/IUserObjectListEntry.h"
#include "CoreMinimal.h"
#include "Delegates/DelegateCombinations.h"
#include "ModioFeaturedCategoryParams.h"
#include "Types/ModioCommonTypes.h"
#include "Types/ModioErrorCode.h"
#include "Types/ModioFilterParams.h"
#include "Types/ModioModInfoList.h"
#include "UI/BaseWidgets/ModioButton.h"
#include "UI/BaseWidgets/ModioRichTextBlock.h"
#include "UI/BaseWidgets/ModioTileView.h"
#include "UI/BaseWidgets/ModioUserWidgetBase.h"
#include "UI/EventHandlers/IModioUIModInfoReceiver.h"
#include "UI/Interfaces/IModioUIAsyncOperationWidget.h"
#include "UI/Views/ModDetails/ModioAsyncOpWrapperWidget.h"

#include "ModioFeaturedCategory.generated.h"

/// Hacky to do it like this but ehhh
DECLARE_DYNAMIC_DELEGATE_RetVal(int32, FOnGetSelectionIndex);

/**
 *
 */
UCLASS(Abstract)
class MODIOUI_API UModioFeaturedCategory : public UModioUserWidgetBase,
										   public IUserObjectListEntry,
										   public IModioUIModInfoReceiver,
										   public IModioUIAsyncOperationWidget
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCategorySelectionChanged, int32, Index, UModioFeaturedCategory*,
												 RealCategory);

protected:
	bool bEmitSelectionEvents = true;

	FOnGetSelectionIndex SelectionIndexDelegate;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioRichTextBlock* Title;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioTileView* ItemList;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidgetOptional, MustImplement = "ModioUIAsyncHandlerWidget"))
	UWidget* TileLoader;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioButton* SeeAllButton;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioButton* NavLeftButton;
	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioButton* NavRightButton;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widgets", meta = (BindWidget))
	UModioAsyncOpWrapperWidget* CategoryViewContent;

	virtual void NativeRequestOperationRetry() override;
	
	UPROPERTY()
	FModioFilterParams Filter;

	void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	virtual void NativeOnSetDataSource() override;

	virtual void NativeOnInitialized() override;
	virtual void NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent) override;

	void OnItemListEntryGenerated(UUserWidget& GeneratedWidget);
	void OnItemListEntryReleased(UUserWidget& GeneratedWidget);
	void OnCategoryFinishedScrolling(UObject* CategoryItem, UUserWidget& CategoryItemWidget);

	UFUNCTION()
	void HandleNavLeftClicked()
	{
		if (ItemList)
		{
			int32 ItemIndex = FMath::Max(ItemList->GetScrollOffset() - ItemList->GetDisplayedEntryWidgets().Num(), 0.f);
			ItemList->NavigateToIndex(ItemIndex);
			ItemList->ClearSelection();
			ItemList->SetSelectedIndex(ItemIndex + ItemList->GetDisplayedEntryWidgets().Num() - 1);
			ItemList->SetScrollOffset(ItemIndex);
		}
	}

	UFUNCTION()
	void HandleNavRightClicked()
	{
		if (ItemList)
		{
			int32 ItemIndex = FMath::Min(ItemList->GetScrollOffset() + ItemList->GetDisplayedEntryWidgets().Num(),
										 (float) ItemList->GetNumItems() - 1);

			ItemList->ClearSelection();
			ItemList->SetSelectedIndex(ItemIndex);
			ItemList->NavigateToIndex(ItemIndex);

			// Only scroll right if we have not reached the end of the category list
			if (ItemIndex <= (ItemList->GetNumItems() - ItemList->GetDisplayedEntryWidgets().Num()))
			{
				ItemList->SetScrollOffset(ItemIndex);
			}
		}
	}

	UFUNCTION()
	void HandleItemListEntrySubscribeClicked(class UModioModInfoUI* ModInfo);

	UFUNCTION()
	void HandleItemListEntryDetailsClicked(class UModioModInfoUI* ModInfo);

	UFUNCTION()
	void HandleItemListEntryReportClicked(class UModioModInfoUI* ModInfo);

	// Could potentially just expose a handler and push the logic into the MainMenu itself which would eliminate the
	// need for a public method on the UI subsystem But at the expense of worse encapsulation the implementation here
	// has more flexibility later.
	UFUNCTION()
	void HandleSeeAllClicked();

	virtual bool NativeSupportsKeyboardFocus() const override;

	FReply NativeOnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent) override;
	FNavigationReply NativeOnNavigation(const FGeometry& InGeometry,
										const FNavigationEvent& InNavigationEvent) override;
	virtual void NativeOnListAllModsRequestCompleted(FString RequestIdentifier, FModioErrorCode ec,
													 TOptional<FModioModInfoList> List) override;
	virtual void OnCategorySelectionChangedInternal(UModioTileView::NullableItemType Item);

	UPROPERTY(BlueprintAssignable, Category = Events, meta = (DisplayName = "On Item Selection Changed"))
	FOnCategorySelectionChanged SelectionChangedDelegate;

public:
	UFUNCTION()
	UModioTileView* const GetItemList() const
	{
		return ItemList;
	}
	FOnCategorySelectionChanged& OnCategorySelectionChanged()
	{
		return SelectionChangedDelegate;
	}

	UFUNCTION(BlueprintCallable, Category="ModioFeaturedCategory")
	void SetSelectionIndexDelegate(FOnGetSelectionIndex Delegate);
	// delegate for See all button
};
