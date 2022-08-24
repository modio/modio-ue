#pragma once

#include "Brushes/SlateColorBrush.h"
#include "Brushes/SlateNoResource.h"
#include "Styling/SlateStyle.h"
#include "Styling/SlateTypes.h"
#include "UI/Styles/ModioUIWidgetStyleContainer.h"
#include "UI/Styles/ModioTextBlockStyleOverride.h"

#include "ModioRichTextStyle.generated.h"

USTRUCT(BlueprintType)
struct MODIOUI_API FModioRichTextStyle : public FSlateWidgetStyle
{
	GENERATED_BODY()

	static const FName TypeName;
	virtual const FName GetTypeName() const override
	{
		return TypeName;
	};

	static const FModioRichTextStyle& GetDefault()
	{
		static FModioRichTextStyle Default;
		return Default;
	}

	static const TArray<FString>& GetSupportedTags()
	{
		static TArray<FString> TagList = {"img", "msdf", "override", "bold"};
		return TagList;
	}


	FModioRichTextStyle() : FSlateWidgetStyle()
	{
		Styles.Add("Default", FTextBlockStyle());
		Styles.Add("Paragraph", FTextBlockStyle());
		Styles.Add("H3", FTextBlockStyle());
		Styles.Add("H2", FTextBlockStyle());
		Styles.Add("H1", FTextBlockStyle());
		Styles.Add("Default_Semibold", FTextBlockStyle());
	}

	TSharedPtr<class FSlateStyleSet> GetStyleSet() const
	{
		TSharedPtr<class FSlateStyleSet> StyleInstance = MakeShared<FSlateStyleSet>("ModioRichTextStyleSet");

		for (const auto& Style : Styles)
		{
			if (!StyleInstance->HasWidgetStyle<FTextBlockStyle>(Style.Key))
			{
				StyleInstance->Set(Style.Key, Style.Value);
			}
			else
			{
				const_cast<FTextBlockStyle&>(StyleInstance->GetWidgetStyle<FTextBlockStyle>(Style.Key)) = Style.Value;
			}
		}

		for (const auto& StyleOverride : InlineStyleOverrides)
		{
			if (!StyleInstance->HasWidgetStyle<FModioTextBlockStyleOverride>(StyleOverride.Key))
			{
				StyleInstance->Set(StyleOverride.Key, StyleOverride.Value);
			}
			else
			{
				const_cast<FModioTextBlockStyleOverride&>(StyleInstance->GetWidgetStyle<FModioTextBlockStyleOverride>(StyleOverride.Key)) = StyleOverride.Value;
			}
		}
		//This also needs to have the FModioTextBlockStyleOverrides added
		return StyleInstance;
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widgets")
	TMap<FName, FTextBlockStyle> Styles;

	/// @brief Inline Style overrides (Should this be moved someplace else perhaps?)
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category="Widgets")
	TMap<FName, FModioTextBlockStyleOverride> InlineStyleOverrides;
};

UCLASS(meta = (DisplayName = "ModioRichTextStyle"))
class UModioRichTextStyleContainer : public UModioUIWidgetStyleContainer
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShowOnlyInnerProperties), Category="Widgets")
	FModioRichTextStyle Style;
	
	virtual void PostLoad() override
	{
		Super::PostLoad();
	}
	
	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return &Style;
	}
};
