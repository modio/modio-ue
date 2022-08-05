// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Algo/Transform.h"
#include "Core/ModioModInfoUI.h"
#include "CoreMinimal.h"
#include "UI/BaseWidgets/ModioButton.h"
#include "UI/BaseWidgets/ModioGridPanel.h"
#include "UI/BaseWidgets/ModioPopupComboBox.h"
#include "UI/BaseWidgets/ModioRichTextBlock.h"
#include "UI/BaseWidgets/ModioTileView.h"
#include "UI/BaseWidgets/ModioUIAsyncLoader.h"
#include "UI/CommonComponents/ModioTagList.h"
#include "UI/EventHandlers/IModioUIModInfoReceiver.h"
#include "UI/Interfaces/IModioUIAsyncOperationWidget.h"
#include "UI/Styles/ModioUIStyleRef.h"
#include "UI/Views/ModioMenuView.h"

#include "ModioSearchResultsView.generated.h"

/**
 *
 */
UCLASS()
class MODIOUI_API UModioSearchResultsView : public UModioMenuView,
											public IModioUIModInfoReceiver,
											public IModioUIAsyncOperationWidget
{
	GENERATED_BODY()
protected:
	UFUNCTION()
	void SortByTrending();

	UFUNCTION()
	void SortByHighestRated();

	UFUNCTION()
	void SortByMostPopular();

	UFUNCTION()
	void SortByMostSubscribers();

	UFUNCTION()
	void SortByRecentlyAdded();

	UFUNCTION()
	void SortByLastUpdated();

	void OnSelectionChanged(FModioUIAction ModioUIAction, ESelectInfo::Type Arg);

	virtual void NativeOnInitialized() override;
	virtual void NativeOnListAllModsRequestCompleted(FString RequestIdentifier, FModioErrorCode ec,
													 TOptional<FModioModInfoList> List) override;
	virtual void NativeOnSetDataSource() override;
	virtual void NativeRequestOperationRetry() override;
	UFUNCTION()
	void OnRefineSearchButtonClicked();

	UPROPERTY()
	TArray<UModioModInfoUI*> SearchResults;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	UModioTileView* ResultsTileView;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets",
			  meta = (BindWidgetOptional, MustImplement = "ModioUIAsyncHandlerWidget"))
	UWidget* ResultLoader;

	UPROPERTY()
	UModioGridPanel* NoResultsDialog;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	UModioRichTextBlock* SearchQuery;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	UModioTagList* SearchTags;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	UModioButton* RefineSearchButton;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	UModioPopupComboBox* SortBy;

	// Override GetMenuTitleContent to return the correct widget with the number of results populated
	virtual TSharedRef<SWidget> GetMenuTitleContent() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (StyleClass = "ModioRichTextStyle"),
			  Category = "ModioSearchResultsView")
	FModioUIStyleRef TitleTextStyle = FModioUIStyleRef {"DefaultRichTextStyle"};

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ModioSearchResultsView")
	FText PageTitle;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ModioSearchResultsView")
	FText ModCountFormatText;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ModioSearchResultsView")
	FText ModQueryFormatText;

	FText GetPageTitle() const;
	FText GetModCountText() const;

	bool HasSortActionApplied;
	FModioUIAction CurrentSortAction;

public:
};
