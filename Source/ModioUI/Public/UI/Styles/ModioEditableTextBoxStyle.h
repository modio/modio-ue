
#pragma once
#include "Styling/SlateTypes.h"
#include "UI/Styles/ModioUIColorRef.h"
#include "UI/Styles/ModioUIWidgetStyleContainer.h"

#include "ModioEditableTextBoxStyle.generated.h"

USTRUCT(BlueprintType)
struct MODIOUI_API FModioEditableTextBoxStyle : public FEditableTextBoxStyle
{
	GENERATED_BODY()
	static const FName TypeName;
	virtual const FName GetTypeName() const override
	{
		return TypeName;
	};
	UPROPERTY(BlueprintReadOnly,EditAnywhere, meta=(InlineEditConditionToggle), Category="Widget")
	bool bShowHintIcon = true;

	UPROPERTY(BlueprintReadOnly,EditAnywhere, meta=(EditCondition=bShowHintIcon), Category="Widget")
	FSlateBrush HintIcon;
	
	UPROPERTY(BlueprintReadOnly,EditAnywhere, Category="Border")
	FSlateBrush NormalBorderBrush;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Border")
	FSlateBrush HoveredBorderBrush;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Border")
	FSlateBrush ErrorBorderBrush;
	
	static const FModioEditableTextBoxStyle& GetDefault()
	{
		static FModioEditableTextBoxStyle Default;
		return Default;
	}

};

UCLASS(meta = (DisplayName = "ModioEditableTextBoxStyle"))
class UModioEditableTextBoxStyleContainer : public UModioUIWidgetStyleContainer
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShowOnlyInnerProperties), Category="Widget")
	FModioEditableTextBoxStyle Style;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return &Style;
	}
};