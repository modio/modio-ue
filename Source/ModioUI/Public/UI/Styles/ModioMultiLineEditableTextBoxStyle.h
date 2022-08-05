#pragma once
#include "Styling/SlateTypes.h"
#include "UI/Styles/ModioUIStyleRef.h"
#include "UI/Styles/ModioUIWidgetStyleContainer.h"

#include "ModioMultiLineEditableTextBoxStyle.generated.h"

USTRUCT(BlueprintType)
struct MODIOUI_API FModioMultiLineEditableTextBoxStyle : public FEditableTextBoxStyle
{
	GENERATED_BODY()
	static const FName TypeName;
	virtual const FName GetTypeName() const override
	{
		return TypeName;
	};

	UPROPERTY(BlueprintReadWrite,EditAnywhere,meta=(StyleClass="ModioTextWidgetStyle"), Category="Widget")
	FModioUIStyleRef TextStyle;

	static const FModioMultiLineEditableTextBoxStyle& GetDefault()
	{
		static FModioMultiLineEditableTextBoxStyle Default;
		return Default;
	}

};

UCLASS(meta = (DisplayName = "ModioMultiLineEditableTextBoxStyle"))
class UModioMultiLineEditableTextBoxStyleContainer : public UModioUIWidgetStyleContainer
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShowOnlyInnerProperties), Category="Widget")
	FModioMultiLineEditableTextBoxStyle Style;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return &Style;
	}
};