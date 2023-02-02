/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/BaseWidgets/ModioEditableText.h"
#include "UI/Styles/ModioRichTextStyle.h"
#include "Widgets/Input/SEditableText.h"

void UModioEditableText::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	if (const FModioRichTextStyle* ResolvedStyle = TextStyle.FindStyle<FModioRichTextStyle>())
	{
		CachedStyleSet = ResolvedStyle->CloneStyleSet();
		if (CachedStyleSet)
		{
			const FTextBlockStyle& SpecificStyle = CachedStyleSet->GetWidgetStyle<FTextBlockStyle>(DefaultStyleName);
			MyEditableText->SetFont(SpecificStyle.Font);
			MyEditableText->SetColorAndOpacity(SpecificStyle.ColorAndOpacity);
		}
	}
}

TArray<FString> UModioEditableText::GetStyleNames() const
{
	TArray<FString> StyleNames;

	const FModioRichTextStyle* RichTextStyle = TextStyle.FindStyle<FModioRichTextStyle>();
	if (RichTextStyle)
	{
		TArray<FName> TmpStyleNames;
		RichTextStyle->Styles.GenerateKeyArray(TmpStyleNames);
		for (const FName& Name : TmpStyleNames)
		{
			StyleNames.Add(Name.ToString());
		}
	}

	if (StyleNames.Num() == 0)
	{
		StyleNames.Add("Default");
	}
	return StyleNames;
}