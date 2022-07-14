#pragma once

#include "Styling/SlateBrush.h"
#include "Styling/SlateTypes.h"
#include "UI/Styles/ModioUIStyleRef.h"
#include "UI/Styles/ModioUIBrushRef.h"
#include "UI/Styles/ModioUIWidgetStyleContainer.h"

#include "ModioDialogStyle.generated.h"

USTRUCT(BlueprintType)
struct MODIOUI_API FModioDialogStyle : public FSlateWidgetStyle
{
	GENERATED_BODY()
	static const FName TypeName;
	virtual const FName GetTypeName() const override
	{
		return TypeName;
	};

	static const FModioDialogStyle& GetDefault()
	{
		static FModioDialogStyle Default;
		return Default;
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMargin ContentPadding;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FSlateBrush BackgroundBrush;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (StyleClass = "ModioRichTextStyle"))
	FModioUIStyleRef TitleTextStyle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (StyleClass = "ModioRichTextStyle"))
	FModioUIStyleRef ContentTextStyle;

	UPROPERTY(BlueprintReadWrite,EditAnywhere, meta = (StyleClass = "ModioButtonStyle"))
	FModioUIStyleRef ButtonStyle;

};

UCLASS(meta = (DisplayName = "ModioDialogStyle"))
class UModioDialogStyleContainer : public UModioUIWidgetStyleContainer
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShowOnlyInnerProperties))
	FModioDialogStyle Style;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return &Style;
	}
};