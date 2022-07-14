#pragma once

#include "Brushes/SlateColorBrush.h"
#include "Brushes/SlateNoResource.h"
#include "Styling/SlateTypes.h"
#include "UI/Styles/ModioWidgetBorderStyle.h"
#include "UI/Styles/ModioUIWidgetStyleContainer.h"

#include "ModioDynamicImageStyle.generated.h"

USTRUCT(BlueprintType)
struct MODIOUI_API FModioDynamicImageStyle : public FSlateWidgetStyle
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
    FSlateBrush ImageLoadingBrush;

    UPROPERTY(BlueprintReadWrite,EditAnywhere)
    FSlateBrush ImageUnavailableBrush;
    
};


UCLASS(meta = (DisplayName = "ModioDynamicImageStyle"))
class UModioDynamicImageStyleContainer : public UModioUIWidgetStyleContainer
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShowOnlyInnerProperties))
	FModioDynamicImageStyle Style;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return &Style;
	}
};