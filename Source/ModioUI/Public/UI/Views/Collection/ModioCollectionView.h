// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core/ModioModCollectionEntryUI.h"
#include "CoreMinimal.h"
#include "UI/BaseWidgets/ModioButton.h"
#include "UI/BaseWidgets/ModioCheckBox.h"
#include "UI/BaseWidgets/ModioEditableTextBox.h"
#include "UI/BaseWidgets/ModioListView.h"
#include "UI/BaseWidgets/ModioPopupComboBox.h"
#include "UI/BaseWidgets/ModioTextBlock.h"
#include "UI/CommonComponents/ModioMenuBar.h"
#include "UI/CommonComponents/ModioRichTextButton.h"
#include "UI/EventHandlers/IModioUIModManagementEventReceiver.h"
#include "UI/EventHandlers/IModioUISubscriptionsChangedReceiver.h"
#include "UI/Views/ModioMenuView.h"

#include "ModioCollectionView.generated.h"

/**
 *
 */
/// TODO: listen for subscription changes etc
UCLASS()
class MODIOUI_API UModioCollectionView : public UModioMenuView,
										 public IModioUISubscriptionsChangedReceiver,
										 public IModioUIModManagementEventReceiver
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioRichTextBlock* CollectionTitle;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioEditableTextBox* SearchInput;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Localization")
	FText ShowUserSubscriptionsText;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Localization")
	FText ShowSystemCollectionText;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioRichTextButton* FetchButton;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioPopupComboBox* ModGroupSelection;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioPopupComboBox* SortBy;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioListView* CollectionList;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioRichTextBlock* CollectionCount;

	/// @brief Ensures that the underlying data set doesn't get GC'd
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widgets")
	TArray<UModioModCollectionEntryUI*> CachedCollection;

	/// @brief Ensures that the underlying data set doesn't get GC'd
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widgets")
	TArray<UModioModCollectionEntryUI*> FilteredCollection;

	FModioUIAction CurrentSortAction;
	bool HasSortActionApplied;

	virtual void NativeOnInitialized() override;

	void OnFetchExternalCompleted(FModioErrorCode ec);

	void UpdateCachedCollection();

	virtual void ApplyFiltersToCollection();
	virtual void NativeOnSubscriptionsChanged(FModioModID ModID, bool bNewSubscriptionState) override;
	virtual void NativeOnModManagementEvent(FModioModManagementEvent Event) override;

	UFUNCTION()
	void SortAToZ();

	UFUNCTION()
	void SortZToA();

	UFUNCTION()
	void SortSizeOnDisk();

	UFUNCTION()
	void SortRecentlyUpdatedDelegate();

	void OnSelectionChanged(FModioUIAction ModioUIAction, ESelectInfo::Type Arg);

	virtual void SynchronizeProperties() override;

	UFUNCTION()
	void OnFetchUpdatesClicked();

	UFUNCTION()
	void OnSearchTextUpdated(const FText& NewText);

	UFUNCTION()
	void OnModGroupChanged(FText SelectedItem, ESelectInfo::Type SelectionType);

public:
};
