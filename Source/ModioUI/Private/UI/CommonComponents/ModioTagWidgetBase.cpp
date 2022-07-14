// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/CommonComponents/ModioTagWidgetBase.h"
#include "Settings/ModioUISettings.h"
#include "UI/Styles/ModioRichTextStyle.h"
#include "UI/Styles/ModioTagWidgetStyle.h"
#include "UI/Styles/ModioUIStyleSet.h"

TSharedRef<SWidget> UModioTagWidgetBase::RebuildWidget()
{
	if (TagText)
	{
		TagText->GetStyleDelegate().BindUObject(this, &UModioTagWidgetBase::GetTagTextStyle);
	}

	return Super::RebuildWidget();
}

void UModioTagWidgetBase::SynchronizeProperties()
{
	if (TagText)
	{
		FFormatNamedArguments Args;
		// NOTE: @modio-core this requires server-side localization of tag categories and values
		Args.Add("Category", FFormatArgumentValue(FText::FromString(TagCategory)));
		Args.Add("Value", FFormatArgumentValue(FText::FromString(TagValue)));
		TagText->SetText(FText::Format(FTextFormat(TagTextFormat), Args));
		if (TagText)
		{
			TagText->GetStyleDelegate().BindUObject(this, &UModioTagWidgetBase::GetTagTextStyle);
			TagText->SynchronizeProperties();
		}
	}
	if (TagBackground)
	{
		if (const FModioTagWidgetStyle* ResolvedStyle = TagStyle.FindStyle<FModioTagWidgetStyle>())
		{
			TagBackground->SetBrush(ResolvedStyle->TagBackgroundBrush);
		}
	}
	Super::SynchronizeProperties();
}

const FModioRichTextStyle& UModioTagWidgetBase::GetTagTextStyle()
{
	if (const FModioTagWidgetStyle* ResolvedStyle = TagStyle.FindStyle<FModioTagWidgetStyle>())
	{
		if (const FModioRichTextStyle* ResolvedLabelStyle =
				ResolvedStyle->TextStyleNew.FindStyle<FModioRichTextStyle>())
		{
			return *ResolvedLabelStyle;
		}
	}
	return FModioRichTextStyle::GetDefault();
}
