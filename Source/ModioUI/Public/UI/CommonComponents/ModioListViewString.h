// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ListViewBase.h"
#include "CoreMinimal.h"
#include "Templates/SharedPointer.h"

#include "ModioListViewString.generated.h"

/**
 *
 */
UCLASS(meta = (EntryInterface = ModioUIUserStringListEntry))
class MODIOUI_API UModioListViewString : public UListViewBase, public ITypedUMGListView<TSharedPtr<FString> >
{
	GENERATED_BODY()

	IMPLEMENT_TYPED_UMG_LIST(TSharedPtr<FString>, MyListView)

protected:
	TArray<TSharedPtr<FString>> ListItems;

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

#if WITH_EDITOR
	virtual void OnRefreshDesignerItems() override;
#endif
	TSharedPtr<SListView<TSharedPtr<FString>> > MyListView;
	virtual TSharedRef<STableViewBase> RebuildListWidget() override;
	virtual void SynchronizeProperties() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	UUserWidget& OnGenerateEntryWidgetInternal(TSharedPtr<FString> Item, TSubclassOf<UUserWidget> DesiredEntryClass,
											   const TSharedRef<STableViewBase>& OwnerTable);
	FMargin GetDesiredEntryPadding(TSharedPtr<FString> Item) const override;

public:
	void SetListItems(const TArray<TSharedPtr<FString>>& InListItems);
};
