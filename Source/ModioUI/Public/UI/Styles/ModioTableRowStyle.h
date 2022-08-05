#pragma once
#include "Materials/MaterialInstanceDynamic.h"
#include "Styling/SlateTypes.h"
#include "UI/Styles/ModioUIColorRef.h"
#include "UI/Styles/ModioUIWidgetStyleContainer.h"
#include "UObject/SoftObjectPtr.h"

#include "ModioTableRowStyle.generated.h"

USTRUCT(BlueprintType)
struct MODIOUI_API FModioTableRowStyle : public FSlateWidgetStyle
{
	GENERATED_BODY()
	static const FName TypeName;
	virtual const FName GetTypeName() const override
	{
		return TypeName;
	};

	static const FModioTableRowStyle& GetDefault()
	{
		static FModioTableRowStyle Default;
		return Default;
	}

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widget")
	FModioUIColorRef BackgroundHoverColor;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widget")
	FModioUIColorRef BackgroundSelectedColor;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widget")
	FModioUIColorRef BackgroundColor;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widget")
	FModioUIColorRef TextHoverColor;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widget")
	FModioUIColorRef TextSelectedColor;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widget")
	FModioUIColorRef TextColor;
};

UCLASS(meta = (DisplayName = "ModioTableRowStyle"))
class UModioTableRowStyleContainer : public UModioUIWidgetStyleContainer
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShowOnlyInnerProperties), Category="Widget")
	FModioTableRowStyle Style;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return &Style;
	}
};