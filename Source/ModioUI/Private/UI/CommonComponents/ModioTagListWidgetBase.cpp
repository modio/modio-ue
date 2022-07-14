// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/CommonComponents/ModioTagListWidgetBase.h"
#include "Core/ModioTagOptionsUI.h"
#include "TimerManager.h"

void UModioTagListWidgetBase::NativeOnSetDataSource()
{
	Super::NativeOnSetDataSource();
	if (TagSelectorList)
	{
		if (UModioTagOptionsUI* TagOptions = Cast<UModioTagOptionsUI>(DataSource))
		{
			TagSelectorList->SetListItems(TagOptions->GetTagCategoriesForUI());
			TagSelectorList->SetScrollOffset(0);
			if (GetWorld())
			{
				GetWorld()->GetTimerManager().SetTimerForNextTick(
					[TagSelectorList = this->TagSelectorList]() { TagSelectorList->RequestRefresh(); });
			}
		}
	}
	InvalidateLayoutAndVolatility();
}

void UModioTagListWidgetBase::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UModioTagListWidgetBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

TArray<FString> UModioTagListWidgetBase::GetSelectedTags()
{
	TArray<FString> CombinedSelectedTags;
	if (UModioTagOptionsUI* TagOptions = Cast<UModioTagOptionsUI>(DataSource))
	{
		for (UModioTagInfoUI* TagInfo : TagOptions->GetTagCategoriesForUI())
		{
			Algo::Transform(TagInfo->SelectedTagValues, CombinedSelectedTags,
							[](TSharedPtr<FString> InString) { return *InString; });
		}
		TagCategoryCount = TagOptions->Underlying.GetRawList().Num();
	}
	return CombinedSelectedTags;
}

void UModioTagListWidgetBase::ClearSelectedTags()
{
	if (UModioTagOptionsUI* TagOptions = Cast<UModioTagOptionsUI>(DataSource))
	{
		for (UModioTagInfoUI* TagInfo : TagOptions->GetTagCategoriesForUI())
		{
			TagInfo->SelectedTagValues.Empty();
		}
		if (TagSelectorList)
		{
			TagSelectorList->RegenerateAllEntries();
		}
	}
}

void UModioTagListWidgetBase::DisplayNextTagCategory()
{
	if (TagSelectorList)
	{
		CurrentlyDisplayedTagCategoryIndex = (CurrentlyDisplayedTagCategoryIndex + 1) % TagCategoryCount;
		TagSelectorList->NavigateToIndex(CurrentlyDisplayedTagCategoryIndex);
	}
}

void UModioTagListWidgetBase::DisplayPrevTagCategory()
{
	if (TagSelectorList)
	{
		if (CurrentlyDisplayedTagCategoryIndex == 0)
		{
			CurrentlyDisplayedTagCategoryIndex = TagCategoryCount - 1;
			TagSelectorList->NavigateToIndex(CurrentlyDisplayedTagCategoryIndex);
		}
	}
}
