#pragma once

#include "Styling/CoreStyle.h"
#include "Styling/SlateTypes.h"
#include "UI/Styles/ModioUIColorRef.h"
#include "UI/Styles/ModioUIStyleRef.h"
#include "UI/Styles/ModioUIWidgetStyleContainer.h"

#include "ModioCustomComboBoxStyle.generated.h"

USTRUCT(BlueprintType)
struct MODIOUI_API FModioCustomComboBoxStyle : public FSlateWidgetStyle
{
	GENERATED_BODY()

	static const FName TypeName;
	virtual const FName GetTypeName() const override
	{
		return TypeName;
	};

	static const FModioCustomComboBoxStyle& GetDefault()
	{
		static FModioCustomComboBoxStyle Default;
		return Default;
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (StyleClass = "ModioRoundedBorderStyle"), Category="Widget")
	FModioUIStyleRef BorderStyle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (StyleClass = "ModioTableRowStyle"), Category="Widget")
	FModioUIStyleRef RowStyle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (StyleClass = "ModioRichTextStyle"), Category="Widget")
	FModioUIStyleRef TextStyle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widget")
	FModioUIColorRef BackgroundColor;
};

UCLASS(meta = (DisplayName = "ModioCustomComboBoxStyle"))
class UModioCustomComboBoxStyleContainer : public UModioUIWidgetStyleContainer
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShowOnlyInnerProperties), Category="Widget")
	FModioCustomComboBoxStyle Style;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return &Style;
	}
};
