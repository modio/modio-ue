/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

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
		// 
		// String tables cannot have spaces in keys so removing spaces from lookup
		UModioUISubsystem* subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (!subsystem)
		{
			Args.Add("Value", FFormatArgumentValue(FText::FromString(TagValue)));
			TagText->SetText(FText::Format(FTextFormat(TagTextFormat), Args));
		}
		else
		{
			TagText->SetText(subsystem->GetLocalizedTag(TagValue));
		}

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
