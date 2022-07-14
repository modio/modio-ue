// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/BaseWidgets/ModioRichTextBlockDecorator.h"
#include "Components/RichTextBlock.h"
#include "Engine/Engine.h"
#include "Fonts/FontMeasure.h"
#include "Framework/Text/SlateTextRun.h"
#include "Framework/Text/SlateWidgetRun.h"
#include "Math/UnrealMathUtility.h"
#include "ModioUISubsystem.h"
#include "Settings/ModioUISettings.h"
#include "UI/Styles/ModioRichTextStyle.h"
#include "UI/Styles/ModioUIStyleSet.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SScaleBox.h"
#include "Widgets/SNullWidget.h"

TSharedPtr<ITextDecorator> UModioRichTextBlockDecorator::CreateDecorator(URichTextBlock* InOwner)
{
	return MakeShared<FModioRichTextDecorator>(InOwner);
}

bool FModioRichTextDecorator::Supports(const FTextRunParseResults& RunParseResult, const FString& Text) const
{
	/*if (FName(RunParseResult.Name) != NAME_None)
	{
		return true;
	}
	return false;*/
	const TArray<FString>& Tags = FModioRichTextStyle::GetSupportedTags();
	return Tags.ContainsByPredicate([Name = RunParseResult.Name](const FString& Entry) {
		return Name.Compare(Entry, ESearchCase::IgnoreCase) == 0;
	});
}

TSharedRef<ISlateRun> FModioRichTextDecorator::Create(const TSharedRef<class FTextLayout>& TextLayout,
													  const FTextRunParseResults& RunParseResult,
													  const FString& OriginalText,
													  const TSharedRef<FString>& InOutModelText,
													  const ISlateStyle* Style)
{
	FTextRange ModelRange;
	ModelRange.BeginIndex = InOutModelText->Len();

	FTextRunInfo RunInfo(RunParseResult.Name,
						 FText::FromString(OriginalText.Mid(RunParseResult.ContentRange.BeginIndex,
															RunParseResult.ContentRange.EndIndex -
																RunParseResult.ContentRange.BeginIndex)));
	for (const TPair<FString, FTextRange>& Pair : RunParseResult.MetaData)
	{
		RunInfo.MetaData.Add(Pair.Key,
							 OriginalText.Mid(Pair.Value.BeginIndex, Pair.Value.EndIndex - Pair.Value.BeginIndex));
	}
	const FTextBlockStyle* TextStyle = &FTextBlockStyle::GetDefault();
	
	if (Style->HasWidgetStyle<FTextBlockStyle>(FName("Default")))
	{
		TextStyle = &Style->GetWidgetStyle<FTextBlockStyle>(FName("Default"));
	}
	if (Owner)
	{
		TextStyle = &Owner->GetDefaultTextStyle();
	}

	TSharedPtr<ISlateRun> SlateRun;
	TSharedPtr<SWidget> DecoratorWidget = CreateDecoratorWidget(RunInfo, *TextStyle, Style);
	if (DecoratorWidget.IsValid())
	{
		*InOutModelText += TEXT('\u200B'); // Zero-Width Breaking Space
		ModelRange.EndIndex = InOutModelText->Len();

		const TSharedRef<FSlateFontMeasure> FontMeasure =
			FSlateApplication::Get().GetRenderer()->GetFontMeasureService();
		int32 ImageSize = 32;
		if (const FString* ImageSizeStr = RunInfo.MetaData.Find("size"))
		{
			FCString::Atoi(**ImageSizeStr);
		}
		// Adding style and fontsize elements to the image tags is super shitty at the moment but without overhauling
		// the parser to support nested styles there's no contextual style information, so we have to do the lookup here
		// rather than relying on the default style having the correct values
		FTextBlockStyle ResolvedTextStyleForSizeCalculation = *TextStyle;
		if (const FString* FontName = RunInfo.MetaData.Find("style"))
		{
			if (Style->HasWidgetStyle<FTextBlockStyle>(FName(*FontName)))

			{
				// Create a copy of the predefined style from the styleset
				ResolvedTextStyleForSizeCalculation = Style->GetWidgetStyle<FTextBlockStyle>(FName(*FontName));
			}
		}
		if (const FString* FontSizeString = RunInfo.MetaData.Find("fontsize"))
		{
			// Override the font size based on the tag value
			int32 FontSize = FCString::Atoi(**FontSizeString);
			ResolvedTextStyleForSizeCalculation.SetFontSize(FontSize);
		}
		// Now we've resolved the style in the styleset and set any size override, measure the height of any
		// alphanumerics
		// Yes, we'll have to consider what to do later with non ASCII lang, but right now I have
		// oddly-sized glyphs in our font so I don't want to get the max height for the entire font
		int32 FontHeight = FontMeasure
							   ->Measure("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890",
										 ResolvedTextStyleForSizeCalculation.Font)
							   .Y;
		// How much bigger than the font, is the image?
		int32 Difference = ImageSize - FontHeight;
		// Split the difference and subtract it from the baseline value (move the baseline downwards, so the image is
		// centered instead of being too high because it shares a baseline with the text)
		int16 WidgetBaseline = FontMeasure->GetBaseline(ResolvedTextStyleForSizeCalculation.Font) -
							   FMath::Min(0.0f, ResolvedTextStyleForSizeCalculation.ShadowOffset.Y) - (Difference / 2);

		FSlateWidgetRun::FWidgetRunInfo WidgetRunInfo(DecoratorWidget.ToSharedRef(), WidgetBaseline);
		SlateRun = FSlateWidgetRun::Create(TextLayout, RunInfo, InOutModelText, WidgetRunInfo, ModelRange);
	}
	else
	{
		// Assume there's a text handler if widget is empty, if there isn't one it will just display an empty string
		if (const FString* FontName = RunInfo.MetaData.Find("style"))
		{
			if (Style->HasWidgetStyle<FTextBlockStyle>(FName(*FontName)))

			{
				// Create a copy of the predefined style from the styleset
				TextStyle = &Style->GetWidgetStyle<FTextBlockStyle>(FName(*FontName));
			}
		}
		FTextBlockStyle TempStyle = *TextStyle;
		CreateDecoratorText(RunInfo, TempStyle, *InOutModelText, Style);
		ModelRange.EndIndex = InOutModelText->Len();
		SlateRun = FSlateTextRun::Create(RunInfo, InOutModelText, TempStyle, ModelRange);
	}

	return SlateRun.ToSharedRef();
}

TSharedPtr<SWidget> FModioRichTextDecorator::CreateDecoratorWidget(const FTextRunInfo& RunInfo,
																   const FTextBlockStyle& DefaultTextStyle,
																   const ISlateStyle* StyleSet) const
{
	if (RunInfo.Name == "msdf")
	{
		const FString* Name = RunInfo.MetaData.Find("name");
		if (Name)
		{
			UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
			if (Subsystem)
			{
				UModioUIStyleSet* DefaultStyle = Subsystem->GetDefaultStyleSet();
				if (DefaultStyle)
				{
					int32 ImageSize = 32;
					if (const FString* ImageSizeStr = RunInfo.MetaData.Find("size"))
					{
						FCString::Atoi(**ImageSizeStr);
					}
					const TSharedPtr<FSlateBrush> GlyphBrush =
						DefaultStyle->GetGlyphBrush(FName(*Name), FVector2D(ImageSize, ImageSize));
					if (GlyphBrush)
					{
						// This should use the height of the current style rather than the default I think
						const TSharedRef<FSlateFontMeasure> FontMeasure =
							FSlateApplication::Get().GetRenderer()->GetFontMeasureService();

						FTextBlockStyle ResolvedTextStyleForSizeCalculation = DefaultTextStyle;
						if (const FString* FontName = RunInfo.MetaData.Find("style"))
						{
							if (StyleSet->HasWidgetStyle<FTextBlockStyle>(FName(*FontName)))

							{
								ResolvedTextStyleForSizeCalculation =
									StyleSet->GetWidgetStyle<FTextBlockStyle>(FName(*FontName));
							}
						}
						if (const FString* FontSizeString = RunInfo.MetaData.Find("fontsize"))
						{
							int32 FontSize = FCString::Atoi(**FontSizeString);
							ResolvedTextStyleForSizeCalculation.SetFontSize(FontSize);
						}

						FVector2D Dimensions =
							FontMeasure->Measure("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890",
												 ResolvedTextStyleForSizeCalculation.Font);

						float IconHeight = FMath::Min(Dimensions.Y, GlyphBrush->ImageSize.Y);

						GlyphBrush->SetImageSize(FVector2D(ImageSize, ImageSize));

						return SNew(SBox)[SNew(SScaleBox)

											  .Stretch(EStretch::ScaleToFit)
											  .StretchDirection(EStretchDirection::Both)
											  .VAlign(VAlign_Center)[SNew(SImage).Image(GlyphBrush.Get())]];
						// SNew(SImage)
						//.Image(GlyphBrush.Get());
					}
				}
			}
		}
	}
	const FString* Path = RunInfo.MetaData.Find("stylepath");
	if (Path)
	{
		// if that's the case we can then load the global style and ask it for the resource with that name
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			UModioUIStyleSet* DefaultStyle = Subsystem->GetDefaultStyleSet();
			if (DefaultStyle)
			{
				const FSlateBrush* NamedBrush = DefaultStyle->GetBrush(FName(*Path));
				if (NamedBrush)
				{
					return SNew(SImage).Image(NamedBrush);
				}
			}
		}
	}

	return nullptr;
}

void FModioRichTextDecorator::CreateDecoratorText(const FTextRunInfo& RunInfo, FTextBlockStyle& InOutTextStyle,
												  FString& InOutString, const ISlateStyle* StyleSet) const
{
	FName RunName = FName(RunInfo.Name);
	if (RunName != NAME_None)
	{
		if (StyleSet && (RunName != FName("override")))
		{
			if (StyleSet->HasWidgetStyle<FModioTextBlockStyleOverride>(RunName))
			{
				const FModioTextBlockStyleOverride& MatchingStyle =
					StyleSet->GetWidgetStyle<FModioTextBlockStyleOverride>(RunName);
				InOutTextStyle = MatchingStyle.ApplyToStyle(InOutTextStyle);
			}
			else if (StyleSet->HasWidgetStyle<FTextBlockStyle>(RunName))
			{
				const FTextBlockStyle& MatchingStyle = StyleSet->GetWidgetStyle<FTextBlockStyle>(RunName);
				InOutTextStyle = MatchingStyle;
			}
			else
			{}
		}
		if (const FString* FontSizeString = RunInfo.MetaData.Find("fontsize"))
		{
			int32 FontSize = FCString::Atoi(**FontSizeString);
			InOutTextStyle.SetFontSize(FontSize);
		}
		if (const FString* FontColorString = RunInfo.MetaData.Find("color"))
		{
			FModioUIColorRef ColorRef = FModioUIColorRef(FName(*FontColorString));
			InOutTextStyle.ColorAndOpacity = ColorRef.ResolveReference();
		}
		if (const FString* BoldString = RunInfo.MetaData.Find("bold"))
		{
			InOutTextStyle.Font.TypefaceFontName = FName("Bold");
		}
		if (RunName == FName("bold"))
		{
			InOutTextStyle.Font.TypefaceFontName = FName("Bold");
		}
	}

	InOutString += RunInfo.Content.ToString();
}
