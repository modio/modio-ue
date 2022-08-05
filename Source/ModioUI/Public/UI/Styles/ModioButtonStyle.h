#pragma once

#include "Brushes/SlateColorBrush.h"
#include "Brushes/SlateNoResource.h"
#include "Styling/SlateTypes.h"
#include "UI/Styles/ModioUIColorRef.h"
#include "UI/Styles/ModioUIStyleRef.h"
#include "UI/Styles/ModioUIWidgetStyleContainer.h"
#include "UI/Styles/ModioWidgetBorderStyle.h"

#include "ModioButtonStyle.generated.h"

USTRUCT(BlueprintType)
struct MODIOUI_API FModioButtonStyle : public FSlateWidgetStyle
{
	GENERATED_BODY()

	static const FName TypeName;

	virtual const FName GetTypeName() const override
	{
		return TypeName;
	};

	static const FModioButtonStyle& GetDefault()
	{
		static FModioButtonStyle Default;
		return Default;
	}

	FModioButtonStyle() : Super()
	{
		ButtonStyle = FButtonStyle()
						  .SetNormal(FSlateColorBrush(FLinearColor(0.25, 0.25, 0.25)))
						  .SetHovered(FSlateColorBrush(FLinearColor(0.5, 0.5, 0.5)))
						  .SetPressed(FSlateColorBrush(FLinearColor(0.5, 0.5, 0.5)));
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widgets")
	FButtonStyle ButtonStyle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (StyleClass = "ModioRichTextStyle"), Category="Widgets")
	FModioUIStyleRef RichTextStyle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (StyleClass = "ModioRoundedBorderStyle"), Category="Widgets")
	FModioUIStyleRef BorderStyle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widgets")
	FMargin ContentPadding;
};

UCLASS(meta = (DisplayName = "ModioButtonStyle"))
class UModioButtonStyleContainer : public UModioUIWidgetStyleContainer
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShowOnlyInnerProperties), Category="Widgets")
	FModioButtonStyle Style;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return &Style;
	}
};
