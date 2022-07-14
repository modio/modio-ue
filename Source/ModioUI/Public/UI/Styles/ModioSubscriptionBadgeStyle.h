#pragma once

#include "Brushes/SlateColorBrush.h"
#include "Brushes/SlateNoResource.h"
#include "Styling/SlateTypes.h"
#include "UI/Styles/ModioUIStyleRef.h"
#include "UI/Styles/ModioWidgetBorderStyle.h"

#include "ModioSubscriptionBadgeStyle.generated.h"

USTRUCT(BlueprintType)
struct MODIOUI_API FModioSubscriptionBadgeStyle : public FSlateWidgetStyle
{
	GENERATED_BODY()

	static const FName TypeName;
	virtual const FName GetTypeName() const override
	{
		return TypeName;
	};

	static const FModioSubscriptionBadgeStyle& GetDefault()
	{
		static FModioSubscriptionBadgeStyle Default;
		return Default;
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTextBlockStyle LabelStyle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (StyleClass = "ModioRichTextStyle"))
	FModioUIStyleRef LabelStyleNew;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (StyleClass = "ModioProgressBarStyle"))
	FModioUIStyleRef ProgressBarStyle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FModioWidgetBorderStyle BadgeBorderStyle;
};

UCLASS(meta = (DisplayName = "ModioSubscriptionBadgeStyle"))
class UModioSubscriptionBadgeStyleContainer : public UModioUIWidgetStyleContainer
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShowOnlyInnerProperties))
	FModioSubscriptionBadgeStyle Style;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return &Style;
	}
};