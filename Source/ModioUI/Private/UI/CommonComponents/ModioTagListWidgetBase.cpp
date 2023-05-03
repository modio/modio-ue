/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/CommonComponents/ModioTagListWidgetBase.h"
#include "UI/CommonComponents/ModioTagSelectorWidgetBase.h"
#include "UI/CommonComponents/ModioTagList.h"
#include "Core/ModioTagOptionsUI.h"
#include "TimerManager.h"
#include "UI/CommonComponents/ModioSelectableTag.h"

void UModioTagListWidgetBase::NativeOnSetDataSource()
{
	Super::NativeOnSetDataSource();
	if (TagSelectorList)
	{
		if (UModioTagOptionsUI* TagOptions = Cast<UModioTagOptionsUI>(DataSource))
		{
			TagSelectorList->SetListItems(TagOptions->GetTagCategoriesForUI());
			TagSelectorList->SetScrollOffset(0);	
			TagSelectorList->SetSelectionMode(ESelectionMode::Single);

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
	bIsFocusable = false;
}

void UModioTagListWidgetBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

TArray<FString> UModioTagListWidgetBase::GetSelectedTags()
{
	TArray<FString> CombinedSelectedTags;
	for (auto& widget : TagSelectorList->GetDisplayedEntryWidgets())
	{
		UModioTagSelectorWidgetBase* tagSelector = Cast<UModioTagSelectorWidgetBase>(widget);
		if (!tagSelector)
		{
			continue;
		}

		for (auto& item : tagSelector->CategoryTagList->GetDisplayedEntryWidgets())
		{
			UModioSelectableTag* tag = Cast<UModioSelectableTag>(item);
			if (tag && tag->IsCheckboxChecked())
			{
				CombinedSelectedTags.Add(tag->GetTagString());
			}
		}
		
	}

	int index = 0;
	for (auto& tag : CombinedSelectedTags) 
	{
		GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Blue, *tag);
		index++;
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
			for (auto& widget : TagSelectorList->GetDisplayedEntryWidgets())
			{
				UModioTagSelectorWidgetBase* tagSelector = Cast<UModioTagSelectorWidgetBase>(widget);
				if (!tagSelector)
				{
					continue;
				}

				for (auto& item : tagSelector->CategoryTagList->GetDisplayedEntryWidgets())
				{
					UModioSelectableTag* tag = Cast<UModioSelectableTag>(item);
					tag->TagSelectedCheckbox->SetCheckedState(ECheckBoxState::Unchecked);
				}
			}
		}
	}
}

