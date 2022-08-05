#pragma once

#include "Brushes/SlateColorBrush.h"
#include "Brushes/SlateNoResource.h"
#include "Styling/SlateTypes.h"
#include "UI/Styles/ModioUIWidgetStyleContainer.h"

#include "ModioProgressBarStyle.generated.h"

USTRUCT(BlueprintType)
struct MODIOUI_API FModioProgressBarStyle : public FProgressBarStyle
{
	GENERATED_BODY()

	FModioProgressBarStyle() : FProgressBarStyle()
	{
		SetBackgroundImage(FSlateColorBrush(FLinearColor(0.25, 0.25, 0.25)));
		SetFillImage(FSlateColorBrush(FLinearColor(0.5, 0.5, 0.5)));
		SetMarqueeImage(FSlateColorBrush(FLinearColor(0.5, 0.5, 0.5)));
	}

	static const FModioProgressBarStyle& GetDefault()
	{
		static FModioProgressBarStyle Default;
		return Default;
	}
};

UCLASS(meta = (DisplayName = "ModioProgressBarStyle"))
class UModioProgressBarStyleContainer : public UModioUIWidgetStyleContainer
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShowOnlyInnerProperties), Category="Widget")
	FModioProgressBarStyle Style;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return &Style;
	}
};
