// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/CommonComponents/ModioTagList.h"
#include "Blueprint/UserWidget.h"
#include "Components/WrapBoxSlot.h"
#include "Types/ModioModTag.h"
#include "UI/BaseWidgets/ModioRichTextBlock.h"
#include "UI/CommonComponents/ModioTagWidgetBase.h"

void UModioTagList::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	UpdateTagWidgets();
}

void UModioTagList::UpdateTagWidgets()
{
	if (*TagWidgetClass)
	{
		ClearChildren();

		EFlowDirectionPreference MyFlowDirectionPreference = FlowDirectionPreference;
		if (FlowDirectionPreference == EFlowDirectionPreference::Culture)
		{
			MyFlowDirectionPreference =
				FLayoutLocalization::GetLocalizedLayoutDirection() == EFlowDirection::RightToLeft
					? EFlowDirectionPreference::RightToLeft
					: EFlowDirectionPreference::LeftToRight;
		}

		if (IsDesignTime())
		{
#if WITH_EDITORONLY_DATA
			if (MyFlowDirectionPreference == EFlowDirectionPreference::RightToLeft &&
				NumberOfPreviewTags > NumberOfTagsToShow)
			{
				UModioRichTextBlock* AdditionalTagsWidget = NewObject<UModioRichTextBlock>(this);
				FFormatNamedArguments Args;
				Args.Add(TEXT("ExtraTagCount"), NumberOfPreviewTags - NumberOfTagsToShow);
				AdditionalTagsWidget->SetText(FText::Format(ExcessTagCountText, Args));
				AddChildToWrapBox(AdditionalTagsWidget)->SetVerticalAlignment(VAlign_Center);
			}
			int32 ClampedTagDisplayCount = FMath::Min(NumberOfPreviewTags, NumberOfTagsToShow);
			for (int32 PreviewTagIndex = 0; PreviewTagIndex < ClampedTagDisplayCount; PreviewTagIndex++)
			{
				UModioTagWidgetBase* TagWidget = CreateWidget<UModioTagWidgetBase>(this, TagWidgetClass);
				TagWidget->SetTagValue(FString("Category"), "RandomTagThatsKindaLong");
				AddChildToWrapBox(TagWidget);
			}
			if (MyFlowDirectionPreference != EFlowDirectionPreference::RightToLeft &&
				NumberOfPreviewTags > NumberOfTagsToShow)
			{
				UModioRichTextBlock* AdditionalTagsWidget = NewObject<UModioRichTextBlock>(this);
				FFormatNamedArguments Args;
				Args.Add(TEXT("ExtraTagCount"), NumberOfPreviewTags - NumberOfTagsToShow);
				AdditionalTagsWidget->SetText(FText::Format(ExcessTagCountText, Args));
				AddChildToWrapBox(AdditionalTagsWidget)->SetVerticalAlignment(VAlign_Center);
			}
#endif
		}
		else
		{
			if (MyFlowDirectionPreference == EFlowDirectionPreference::RightToLeft && Tags.Num() > NumberOfTagsToShow)
			{
				UModioRichTextBlock* AdditionalTagsWidget = NewObject<UModioRichTextBlock>(this);
				FFormatNamedArguments Args;
				Args.Add(TEXT("ExtraTagCount"), Tags.Num() - NumberOfTagsToShow);
				AdditionalTagsWidget->SetText(FText::Format(ExcessTagCountText, Args));
				AddChildToWrapBox(AdditionalTagsWidget)->SetVerticalAlignment(VAlign_Center);
			}
			int32 ClampedTagDisplayCount = FMath::Min(Tags.Num(), NumberOfTagsToShow);
			for (int32 DisplayTagIndex = 0; DisplayTagIndex < ClampedTagDisplayCount; DisplayTagIndex++)
			{
				const FModioModTag& CurrentTag = Tags[DisplayTagIndex];
				UModioTagWidgetBase* TagWidget = CreateWidget<UModioTagWidgetBase>(this, TagWidgetClass);
				TagWidget->SetTagValue(GetTagGroupNameForTagValue(CurrentTag.Tag), CurrentTag.Tag);
				AddChildToWrapBox(TagWidget);
			}
			if (MyFlowDirectionPreference != EFlowDirectionPreference::RightToLeft && Tags.Num() > NumberOfTagsToShow)
			{
				UModioRichTextBlock* AdditionalTagsWidget = NewObject<UModioRichTextBlock>(this);
				FFormatNamedArguments Args;
				Args.Add(TEXT("ExtraTagCount"), Tags.Num() - NumberOfTagsToShow);
				AdditionalTagsWidget->SetText(FText::Format(ExcessTagCountText, Args));
				AddChildToWrapBox(AdditionalTagsWidget)->SetVerticalAlignment(VAlign_Center);
			}
		}
	}
}

FString UModioTagList::GetTagGroupNameForTagValue(FString TagValue)
{
	if (TagOptions.IsSet())
	{
		for(auto TagOption : TagOptions.GetValue().InternalList)
		{
			if (TagOption.TagGroupValues.Contains(TagValue))
			{
				return TagOption.TagGroupName;
			}
		}
	}

	return FString();
	
}

void UModioTagList::SetTags(TArray<FModioModTag> NewTags)
{
	if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		TagOptions = Subsystem->GetTagOptionsList();
	}
	
	Tags = MoveTemp(NewTags);
	UpdateTagWidgets();
}
