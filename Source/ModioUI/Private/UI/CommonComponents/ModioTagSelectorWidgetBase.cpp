#include "UI/CommonComponents/ModioTagSelectorWidgetBase.h"
#include "Algo/Transform.h"
#include "Core/ModioTagOptionsUI.h"
#include "Types/ModioModTagInfo.h"

void UModioTagSelectorWidgetBase::NativeOnInitialized()
{
	if (CategoryTagList)
	{
		CategoryTagList->OnItemSelectionChanged().AddUObject(this, &UModioTagSelectorWidgetBase::OnTagSelectionChanged);
	}
}

void UModioTagSelectorWidgetBase::OnTagSelectionChanged(TSharedPtr<FString>)
{
	UModioTagInfoUI* TagInfo = Cast<UModioTagInfoUI>(DataSource);
	if (TagInfo)
	{
		TArray<TSharedPtr<FString>> SelectedTagsForCategory;
		CategoryTagList->GetSelectedItems(SelectedTagsForCategory);
		TagInfo->SelectedTagValues = SelectedTagsForCategory;
	}
}

void UModioTagSelectorWidgetBase::NativeOnSetDataSource()
{
	Super::NativeOnSetDataSource();
	Refresh();
}

void UModioTagSelectorWidgetBase::Refresh()
{
	UModioTagInfoUI* TagInfo = Cast<UModioTagInfoUI>(DataSource);
	if (TagInfo)
	{
		if (TagCategoryLabel)
		{
			TagCategoryLabel->SetText(FText::FromString(TagInfo->Underlying.TagGroupName));
		}
		if (CategoryTagList)
		{
			TagListItemSource.Empty();
			Algo::Transform(TagInfo->Underlying.TagGroupValues, TagListItemSource,
							[](FString Value) { return MakeShared<FString>(Value); });
			CategoryTagList->SetListItems(TagListItemSource);

			for (const auto& Tag : TagInfo->SelectedTagValues)
			{
				CategoryTagList->SetItemSelection(Tag, true, ESelectInfo::Direct);
			}
		}
	}
}

void UModioTagSelectorWidgetBase::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	SetDataSource(ListItemObject);
}

TArray<FString> UModioTagSelectorWidgetBase::GetSelectedTags()
{
	TArray<FString> ConvertedTagList;
	if (UModioTagInfoUI* TagInfo = Cast<UModioTagInfoUI>(DataSource))
	{
		Algo::Transform(TagInfo->SelectedTagValues, ConvertedTagList,
						[](TSharedPtr<FString> InString) { return *InString; });
	}
	return ConvertedTagList;
}

void UModioTagSelectorWidgetBase::ClearSelectedTags()
{
	if (UModioTagInfoUI* TagInfo = Cast<UModioTagInfoUI>(DataSource))
	{
		TagInfo->SelectedTagValues.Empty();
	}
	Refresh();
}
