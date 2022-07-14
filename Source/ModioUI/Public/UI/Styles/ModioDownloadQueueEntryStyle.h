#pragma once

#include "Brushes/SlateColorBrush.h"
#include "Brushes/SlateNoResource.h"
#include "Styling/SlateTypes.h"
#include "UI/Styles/ModioUIStyleRef.h"
#include "UI/Styles/ModioUIWidgetStyleContainer.h"

#include "ModioDownloadQueueEntryStyle.generated.h"

USTRUCT(BlueprintType)
struct MODIOUI_API FModioDownloadQueueEntryStyle : public FSlateWidgetStyle
{
	GENERATED_BODY()

	static const FName TypeName;
	virtual const FName GetTypeName() const override
	{
		return TypeName;
	};

	static const FModioDownloadQueueEntryStyle& GetDefault()
	{
		static FModioDownloadQueueEntryStyle Default;
		return Default;
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FSlateBrush HighlightedBorderBrush;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (StyleClass = "ModioButtonStyle"))
	FModioUIStyleRef UnsubscribeButtonStyle = FModioUIStyleRef {"DefaultButtonStyle"};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (StyleClass = "ModioRichTextStyle"))
	FModioUIStyleRef TextStyle = FModioUIStyleRef {"DefaultRichTextStyle"};
};

UCLASS(meta = (DisplayName = "ModioDownloadQueueEntryStyle"))
class UModioDownloadQueueEntryStyleContainer : public UModioUIWidgetStyleContainer
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShowOnlyInnerProperties))
	FModioDownloadQueueEntryStyle Style;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return &Style;
	}
};
