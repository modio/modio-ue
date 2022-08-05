#pragma once

#include "Brushes/SlateColorBrush.h"
#include "Brushes/SlateNoResource.h"
#include "Styling/SlateTypes.h"
#include "UI/Styles/ModioRichTextStyle.h"
#include "UI/Styles/ModioUIStyleRef.h"
#include "UI/Styles/ModioWidgetBorderStyle.h"

#include "ModioTagWidgetStyle.generated.h"

USTRUCT(BlueprintType)
struct MODIOUI_API FModioTagWidgetStyle : public FSlateWidgetStyle
{
	GENERATED_BODY()

	static const FName TypeName;
	virtual const FName GetTypeName() const override
	{
		return TypeName;
	};

	static const FModioTagWidgetStyle& GetDefault()
	{
		static FModioTagWidgetStyle Default;
		return Default;
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widget", meta = (StyleClass = "ModioRichTextStyle"))
	FModioUIStyleRef TextStyleNew;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widget")
	FSlateBrush TagBackgroundBrush;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widget")
	FModioRichTextStyle TextStyle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widget")
	FModioWidgetBorderStyle TagBackgroundStyle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widget")
	FMargin ContentPadding;
};

UCLASS(meta = (DisplayName = "ModioTagWidgetStyle"))
class UModioTagWidgetStyleContainer : public UModioUIWidgetStyleContainer
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widget", meta = (ShowOnlyInnerProperties))
	FModioTagWidgetStyle Style;

	virtual void PostLoad() override
	{
		Super::PostLoad();
	}

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return &Style;
	}
};
