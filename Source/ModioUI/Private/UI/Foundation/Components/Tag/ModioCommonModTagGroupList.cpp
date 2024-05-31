/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Foundation/Components/Tag/ModioCommonModTagGroupList.h"

#include "Components/PanelWidget.h"
#include "UI/Foundation/Components/Tag/ModioCommonModTagEntry.h"
#include "UI/Foundation/Components/Text/TextBlock/ModioCommonTextBlock.h"

bool UModioCommonModTagGroupList::SetSelectedTagGroupValues_Implementation(const TArray<FString>& TagGroupValues, bool bSelect)
{
	TSet<FString> TagGroupValuesToSelectSet(TagGroupValues);
	if (TagsContainer)
	{
		for (UWidget* Child : TagsContainer->GetAllChildren())
		{
			if (UModioCommonModTagEntry* ModTagEntry = Cast<UModioCommonModTagEntry>(Child))
			{
				if (TagGroupValuesToSelectSet.Find(ModTagEntry->GetTag()))
				{
					ModTagEntry->SetSelected(bSelect);
				}
			}
		}
		return true;
	}
	return false;
}

bool UModioCommonModTagGroupList::GetSelectedTagGroupValues_Implementation(TArray<FString>& OutSelectedTagGroupValues) const
{
	if (TagsContainer)
	{
		for (UWidget* Child : TagsContainer->GetAllChildren())
		{
			if (UModioCommonModTagEntry* ModTagEntry = Cast<UModioCommonModTagEntry>(Child))
			{
				if (ModTagEntry->GetSelected())
				{
					OutSelectedTagGroupValues.Add(ModTagEntry->GetTag());
				}
			}
		}
		return true;
	}
	return false;
}

void UModioCommonModTagGroupList::SetTagsGroup_Implementation(const FModioModTagInfo& TagInfo)
{
	if (TagGroupNameTextBlock)
	{
		TagGroupNameTextBlock->SetText(FText::FromString(TagInfo.LocalizedTagCategoryDataCurrentLocale.GroupName));
	}

	if (TagsContainer && TagEntryClass)
	{
		TagsContainer->ClearChildren();
		for (const FString& Tag : TagInfo.LocalizedTagCategoryDataCurrentLocale.Tags)
		{
			UModioCommonModTagEntry* Entry = CreateWidget<UModioCommonModTagEntry>(TagsContainer.Get(), TagEntryClass);
			if (Entry) 
			{
				Entry->SetTag(Tag);
				Entry->SetAllowMultipleSelection(TagInfo.bAllowMultipleSelection);
				Entry->OnModTagEntrySelectionStateChanged.AddWeakLambda(this, [this](UModioCommonModTagEntry* ModTagEntry, bool bIsSelected)
				{
					UpdateSelection(ModTagEntry, bIsSelected);
				});
				TagsContainer->AddChild(Entry);
			}
		}
	}
}

void UModioCommonModTagGroupList::ResetTagsSelection_Implementation()
{
	if (TagsContainer)
	{
		for (UWidget* Child : TagsContainer->GetAllChildren())
		{
			if (UModioCommonModTagEntry* ModTagEntry = Cast<UModioCommonModTagEntry>(Child))
			{
				ModTagEntry->SetSelected(false);
			}
		}
	}
}

void UModioCommonModTagGroupList::UpdateSelection_Implementation(UModioCommonModTagEntry* ModTagEntry, bool bIsSelected)
{
	if (!ModTagEntry || !bIsSelected || ModTagEntry->GetAllowMultipleSelection())
	{
		return;
	}

	if (TagsContainer)
	{
		for (UWidget* Child : TagsContainer->GetAllChildren())
		{
			if (UModioCommonModTagEntry* IteratingModTagEntry = Cast<UModioCommonModTagEntry>(Child))
			{
				if (IteratingModTagEntry != ModTagEntry)
				{
					IteratingModTagEntry->SetSelected(false);
				}
			}
		}
	}
}

UWidget* UModioCommonModTagGroupList::NativeGetDesiredFocusTarget() const
{
	if (UWidget* WidgetToFocus = Super::NativeGetDesiredFocusTarget())
	{
		return WidgetToFocus;
	}
	if (TagsContainer)
	{
		for (UWidget* ChildWidget : TagsContainer->GetAllChildren())
		{
			if (UModioCommonModTagEntry* ModTagEntry = Cast<UModioCommonModTagEntry>(ChildWidget))
			{
				UWidget* WidgetToFocus = ModTagEntry->GetDesiredFocusTarget();
				if (ModTagEntry->GetSelected() && WidgetToFocus)
				{
					return WidgetToFocus;
				}
			}
		}

		if (UModioCommonModTagEntry* FirstModTagEntry = Cast<UModioCommonModTagEntry>(TagsContainer->GetChildAt(0)))
		{
			return FirstModTagEntry->GetDesiredFocusTarget();
		}
	}
	return nullptr;
}

void UModioCommonModTagGroupList::SynchronizeProperties()
{
	Super::SynchronizeProperties();

#if WITH_EDITOR
	if (IsDesignTime() && TagsContainer && !TagsContainer->HasAnyChildren())
	{
		SetTagsGroup(PreviewModTagInfo);
	}
#endif
}
