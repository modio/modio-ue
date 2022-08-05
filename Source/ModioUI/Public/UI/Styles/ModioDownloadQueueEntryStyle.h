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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widget")
	FSlateBrush HighlightedBorderBrush;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (StyleClass = "ModioButtonStyle"), Category="Widget")
	FModioUIStyleRef UnsubscribeButtonStyle = FModioUIStyleRef {"DefaultButtonStyle"};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (StyleClass = "ModioRichTextStyle"), Category="Widget")
	FModioUIStyleRef TextStyle = FModioUIStyleRef {"DefaultRichTextStyle"};
};

UCLASS(meta = (DisplayName = "ModioDownloadQueueEntryStyle"))
class UModioDownloadQueueEntryStyleContainer : public UModioUIWidgetStyleContainer
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShowOnlyInnerProperties), Category="Widget")
	FModioDownloadQueueEntryStyle Style;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return &Style;
	}
};
