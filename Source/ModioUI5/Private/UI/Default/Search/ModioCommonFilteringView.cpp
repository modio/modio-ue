/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Default/Search/ModioCommonFilteringView.h"

#include "ModioSubsystem.h"
#include "Components/PanelWidget.h"
#include "Engine/Engine.h"
#include "UI/Foundation/Components/Tag/ModioCommonModTagGroupList.h"
#include "UI/Foundation/Components/Tag/ModioCommonModTagList.h"

UModioCommonFilteringView::UModioCommonFilteringView()
{
	bAutoFocusOnActivation = false;
}

bool UModioCommonFilteringView::GetSelectedTagGroupValues_Implementation(TArray<FString>& OutSelectedTagGroupValues) const
{
	if (FilteringTagsContainer)
	{
		for (UWidget* ChildWidget : FilteringTagsContainer->GetAllChildren())
		{
			if (UModioCommonModTagGroupList* ModTagGroupList = Cast<UModioCommonModTagGroupList>(ChildWidget))
			{
				ModTagGroupList->GetSelectedTagGroupValues(OutSelectedTagGroupValues);
			}
		}
		return true;
	}
	return false;
}

void UModioCommonFilteringView::ResetFiltering_Implementation()
{
	if (FilteringTagsContainer)
	{
		for (UWidget* ChildWidget : FilteringTagsContainer->GetAllChildren())
		{
			if (UModioCommonModTagGroupList* ModTagGroupList = Cast<UModioCommonModTagGroupList>(ChildWidget))
			{
				ModTagGroupList->ResetTagsSelection();
			}
		}
	}
}

void UModioCommonFilteringView::AddModTagInfo_Implementation(const FModioModTagInfo& ModTagInfo)
{
	if (FilteringTagsContainer && TagGroupListClass)
	{
		UModioCommonModTagGroupList* ModTagGroupList = CreateWidget<UModioCommonModTagGroupList>(FilteringTagsContainer.Get(), TagGroupListClass);
		ModTagGroupList->SetTagsGroup(ModTagInfo);
		FilteringTagsContainer->AddChild(ModTagGroupList);
	}
}

void UModioCommonFilteringView::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (FilteringTagsContainer)
	{
		FilteringTagsContainer->ClearChildren();
	}

	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->GetModTagOptionsAsync(
			FOnGetModTagOptionsDelegateFast::CreateWeakLambda(this, [this](FModioErrorCode ErrorCode, TOptional<FModioModTagOptions> Options)
			{
				if (!ErrorCode && Options.IsSet())
				{
					for (const FModioModTagInfo& TagInfo : Options.GetValue().GetRawList())
					{
						AddModTagInfo(TagInfo);
					}
				}
			}));
	}
}

UWidget* UModioCommonFilteringView::NativeGetDesiredFocusTarget() const
{
	if (UWidget* WidgetToFocus = Super::NativeGetDesiredFocusTarget())
	{
		return WidgetToFocus;
	}
	if (FilteringTagsContainer)
	{
		for (UWidget* ChildWidget : FilteringTagsContainer->GetAllChildren())
		{
			if (UModioCommonModTagGroupList* ModTagGroupList = Cast<UModioCommonModTagGroupList>(ChildWidget))
			{
				if (UWidget* WidgetToFocus = ModTagGroupList->GetDesiredFocusTarget())
				{
					return WidgetToFocus;
				}
			}
		}
	}
	return nullptr;
}

void UModioCommonFilteringView::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	
#if WITH_EDITOR
	if (IsDesignTime())
	{
		if (FilteringTagsContainer)
		{
			FilteringTagsContainer->ClearChildren();
		}
		for (const FModioModTagInfo& TagInfo : PreviewModTagInfoOptions)
		{
			AddModTagInfo(TagInfo);
		}
	}
#endif
}
