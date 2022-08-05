// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ListViewBase.h"
#include "CoreMinimal.h"
#include "Templates/SharedPointer.h"

#include "ModioListViewInteger.generated.h"

/**
 *
 */
UCLASS(meta = (EntryInterface = UserObjectListEntry))
class MODIOUI_API UModioListViewInteger : public UListViewBase, public ITypedUMGListView<TSharedPtr<int64> >
{
	GENERATED_BODY()

	IMPLEMENT_TYPED_UMG_LIST(TSharedPtr<int64>, MyListView)

protected:
	TArray<TSharedPtr<int64>> ListItems;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ListView)
	TEnumAsByte<EOrientation> Orientation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ListView)
	ESlateVisibility ScrollbarVisibility;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ClampMin = 0), Category="ListView")
	float EntrySpacing = 0.f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="ListView")
	bool bOverrideItemSize;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (EditCondition = bOverrideItemSize), Category="ListView")
	FVector2D ItemSize;

	void ResetSelectionStateOnDisplayedWidgets();

#if WITH_EDITOR
	virtual void OnRefreshDesignerItems() override;
#endif
	TSharedPtr<SListView<TSharedPtr<int64>> > MyListView;
	virtual TSharedRef<STableViewBase> RebuildListWidget() override;
	virtual void SynchronizeProperties() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	UUserWidget& OnGenerateEntryWidgetInternal(TSharedPtr<int64> Item, TSubclassOf<UUserWidget> DesiredEntryClass,
											   const TSharedRef<STableViewBase>& OwnerTable);
	FMargin GetDesiredEntryPadding(TSharedPtr<int64> Item) const override;

public:
	void SetListItems(const TArray<TSharedPtr<int64>>& InListItems);
	void SetItemSelection(int32 Index, bool bSelectionState);
	void SelectSingleItem(int32 Index);

};
