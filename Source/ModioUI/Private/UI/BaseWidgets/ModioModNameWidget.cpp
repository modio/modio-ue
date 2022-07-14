// Fill out your copyright notice in the Description page of Project Settings.

#include "ModioModNameWidget.h"

#include "UI/Interfaces/IModioModInfoUIDetails.h"
#include "UI/Styles/ModioDialogStyle.h"
#include "UI/Styles/ModioRichTextStyle.h"

#include "UI/BaseWidgets/Slate/SModioRichTextBlock.h"

void UModioModNameWidget::SynchronizeProperties()
{
	UWidget::SynchronizeProperties();

	const FModioDialogStyle* ResolvedStyle = Style.FindStyle<FModioDialogStyle>();

	if (ResolvedStyle)
	{
		const FModioRichTextStyle* ResolvedTitleTextStyle =
			ResolvedStyle->ContentTextStyle.FindStyle<FModioRichTextStyle>();

		if (ResolvedTitleTextStyle)
		{
			MyText->StyleReference = &ResolvedStyle->ContentTextStyle;
			MyText->ReapplyStyle();
		}
	}
}

TSharedRef<SWidget> UModioModNameWidget::RebuildWidget()
{
	if (DataSource)
	{
		if (DataSource->Implements<UModioModInfoUIDetails>())
		{
			FString ModName = FString::Printf(TEXT("<override bold=\"true\">'%s'</>"),
											  *IModioModInfoUIDetails::Execute_GetFullModInfo(DataSource).ProfileName);
			return SAssignNew(MyText, SModioRichTextBlock).Text(FText::FromString(ModName));
		}
	}

	return SAssignNew(MyText, SModioRichTextBlock).Text(FText::FromString("Mod Name"));
}