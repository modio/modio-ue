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

#include "Algo/Transform.h"
#include "Core/ModioModInfoUI.h"
#include "CoreMinimal.h"
#include "UI/CommonComponents/ModioTagList.h"
#include "UI/Commands/ModioUIAction.h"
#include "UI/EventHandlers/IModioUIModInfoReceiver.h"
#include "UI/Interfaces/IModioUIAsyncOperationWidget.h"
#include "UI/Interfaces/IModioUIAsyncRetryWidget.h"
#include "UI/Styles/ModioUIStyleRef.h"
#include "UI/Views/ModioMenuView.h"
#include "ModioSearchResultsView.generated.h"

/**
 *
 */
UCLASS()
class MODIOUI_API UModioSearchResultsView : public UModioMenuView,
											public IModioUIModInfoReceiver,
											public IModioUIAsyncOperationWidget,
											public IModioUIAsyncRetryWidget
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

	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;

	virtual void NativeOnSetDataSource() override;

	virtual void NativeRequestOperationRetry() override;

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent);

	virtual FReply NativeOnPreviewKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;

	FReply TryNavigateGrid();

	UFUNCTION()
	void OnRefineSearchButtonClicked();

	UFUNCTION()
	void OnRetryPressed();

	UPROPERTY()
	TArray<UModioModInfoUI*> SearchResults;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	class UModioTileView* ResultsTileView;

	// This is initialized on blueprints, since it was a lot simpler there
	UPROPERTY(BlueprintReadWrite, Category = "Widgets")
	class UUserWidget* FirstTile;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets",
			  meta = (BindWidgetOptional, MustImplement = "ModioUIAsyncHandlerWidget"))
	class UWidget* ResultLoader;

	UPROPERTY()
	class UModioGridPanel* NoResultsDialog;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	class UModioTagList* SearchTags;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	class UModioButton* RefineSearchButton;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	class UModioButton* NoResultsRefineSearchButton;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	class UModioPopupComboBox* SortBy;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget))
	class UModioErrorRetryWidget* ModioErrorWithRetryWidget;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	UModioRichTextBlock* SearchResultsCount;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (StyleClass = "ModioRichTextStyle"),
			  Category = "ModioSearchResultsView")
	FModioUIStyleRef TitleTextStyle = FModioUIStyleRef {"DefaultRichTextStyle"};

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ModioSearchResultsView")
	FText PageTitle;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ModioSearchResultsView")
	FText ModQueryFormatText;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	FString DefaultTagText;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ModioSearchResultsView")
	FText SearchInputTagFormatText;

	FText GetPageTitle() const;

	bool HasSortActionApplied;
	FModioUIAction CurrentSortAction;
	FOnRetryRequested RetryDelegate;
	int CurrentListIndex = 0;
	int CurrentRow = 0;

	TArray<FModioModTag> DefaultTags;
	TArray<FString> TagNames;
	FString SearchInputString;

	UFUNCTION()
	void OnDrawerClosed();

public:
	void SetSearchInputString(FString input);
};
