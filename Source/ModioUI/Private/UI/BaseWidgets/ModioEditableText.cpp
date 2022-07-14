// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/BaseWidgets/ModioEditableText.h"
#include "UI/Styles/ModioRichTextStyle.h"
#include "Widgets/Input/SEditableText.h"

void UModioEditableText::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	if (const FModioRichTextStyle* ResolvedStyle = TextStyle.FindStyle<FModioRichTextStyle>())
	{
		TSharedPtr<FSlateStyleSet> StyleSet = ResolvedStyle->GetStyleSet();
		if (StyleSet)
		{
			const FTextBlockStyle& SpecificStyle = StyleSet->GetWidgetStyle<FTextBlockStyle>(DefaultStyleName);
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