/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Foundation/Components/Tag/ModioCommonModTagList.h"
#include "Algo/Transform.h"
#include "Misc/EngineVersionComparison.h"
#include "Types/ModioModInfo.h"
#include "UI/Foundation/Components/List/ModioCommonListView.h"
#include "UI/Foundation/Components/List/ModioCommonTileView.h"
#include "UI/Foundation/Components/Tag/ModioCommonModTagEntry.h"
#include "UI/Foundation/Components/WrapBox/ModioCommonWrapBox.h"

void UModioCommonModTagList::SetTags_Implementation(const TArray<FModioModTag>& ModTags)
{
	TArray<FString> Tags;
	Algo::Transform(ModTags, Tags, [](const FModioModTag& Tag) { return Tag.TagLocalized.ToString(); });
	SetTagsString(Tags);
}

void UModioCommonModTagList::SetTagsString_Implementation(const TArray<FString>& Tags)
{
	if (TagsContainer && TagEntryClass)
	{
		TagsContainer->ClearChildren();
		for (const FString& Tag : Tags)
		{
			UModioCommonModTagEntry* Entry = CreateWidget<UModioCommonModTagEntry>(TagsContainer.Get(), TagEntryClass);
			if (Entry)
			{
				Entry->SetTag(Tag);
				TagsContainer->AddChild(Entry);
			}
		}
	}
}

UWidget* UModioCommonModTagList::NativeGetDesiredFocusTarget() const
{
	if (UWidget* WidgetToFocus = BP_GetDesiredFocusTarget())
	{
		return WidgetToFocus;
	}

	if (TagsContainer)
	{
		for (UWidget* Child : TagsContainer->GetAllChildren())
		{
			if (UModioCommonActivatableWidget* ActivatableWidget = Cast<UModioCommonActivatableWidget>(Child))
			{
				if (UWidget* WidgetToFocus = ActivatableWidget->GetDesiredFocusTarget())
				{
					return WidgetToFocus;
				}
			}
		}

		for (UWidget* Child : TagsContainer->GetAllChildren())
		{
			if (UUserWidget* UserWidget = Cast<UUserWidget>(Child))
			{
#if UE_VERSION_OLDER_THAN(5, 2, 0)
				if (UserWidget->bIsFocusable)
#else
				if (UserWidget->IsFocusable())
#endif
				{
					return UserWidget;
				}
			}
		}
	}

	return nullptr;
}
