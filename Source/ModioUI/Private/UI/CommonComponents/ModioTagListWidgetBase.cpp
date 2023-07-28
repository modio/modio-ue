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
#include "Components/ScrollBox.h"
#include "Components/VerticalBox.h"
#include "Core/ModioTagOptionsUI.h"
#include "TimerManager.h"
#include "Engine.h"
#include "UI/CommonComponents/ModioSelectableTag.h"

void UModioTagListWidgetBase::NativeOnSetDataSource()
{
	Super::NativeOnSetDataSource();
	for (auto& widget : SelectorListScrollBox->GetAllChildren())
	{
		SelectorListScrollBox->RemoveChild(widget);
	}

	if (UModioTagOptionsUI* TagOptions = Cast<UModioTagOptionsUI>(DataSource))
	{
		for (auto& item : TagOptions->GetTagCategoriesForUI())
		{
			UModioTagSelectorWidgetBase* widget =
				CreateWidget<UModioTagSelectorWidgetBase>(this, TagSelectorListTemplate);
			SelectorListScrollBox->AddChild(widget);
			widget->SetDataSource(item);
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
	for (auto& widget : SelectorListScrollBox->GetAllChildren())
	{
		UModioTagSelectorWidgetBase* tagSelector = Cast<UModioTagSelectorWidgetBase>(widget);
		if (!tagSelector)
		{
			continue;
		}

		for (auto& item : tagSelector->CategoryVerticalBox->GetAllChildren())
		{
			UModioSelectableTag* tag = Cast<UModioSelectableTag>(item);
			if (tag && tag->IsCheckboxChecked())
			{
				CombinedSelectedTags.Add(tag->SearchString);
			}
		}
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
		for (auto& widget : SelectorListScrollBox->GetAllChildren())
		{
			UModioTagSelectorWidgetBase* tagSelector = Cast<UModioTagSelectorWidgetBase>(widget);
			if (!tagSelector)
			{
				continue;
			}

			if (tagSelector->IsCollapsed())
			{
				tagSelector->OnCategoryCollapseToggled();
			}

			for (auto& item : tagSelector->CategoryVerticalBox->GetAllChildren())
			{
				UModioSelectableTag* tag = Cast<UModioSelectableTag>(item);
				tag->TagSelectedCheckbox->SetCheckedState(ECheckBoxState::Unchecked);
			}
		}
	}
}

