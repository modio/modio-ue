#pragma once
#include "Styling/SlateTypes.h"
#include "UI/Styles/ModioUIColorRef.h"
#include "UI/Styles/ModioUIWidgetStyleContainer.h"

#include "ModioCodeInputStyle.generated.h"


//Possibly just reference an existing EditableTextBoxStyle instead 
USTRUCT(BlueprintType)
struct MODIOUI_API FModioCodeInputStyle : public FTextBlockStyle
{
	GENERATED_BODY()
	static const FName TypeName;
	virtual const FName GetTypeName() const override
	{
		return TypeName;
	};
	
	//UPROPERTY(BlueprintReadWrite,EditAnywhere, meta=(StyleClass="ModioEditableTextBoxStyle"))
	//FModioUIStyleRef TextBoxStyle;

	/// @brief Brush to use for the box around each character element
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category="Widget")
	FSlateBrush CharacterBrush;

	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category="Widget")
	FModioUIColorRef ColorWhenFocused;

	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category="Widget")
	FModioUIMaterialRef FakeCaretMaterial;

	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category="Widget")
	FMargin CharacterSpacing;

	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category="Widget")
	FVector2D MinimumCharacterSize;

	static const FModioCodeInputStyle& GetDefault()
	{
		static FModioCodeInputStyle Default;
		return Default;
	}

};

UCLASS(meta = (DisplayName = "ModioCodeInputStyle"))
class UModioCodeInputStyleContainer : public UModioUIWidgetStyleContainer
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShowOnlyInnerProperties), Category="Widget")
	FModioCodeInputStyle Style;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return &Style;
	}
	virtual void PostLoad() override
	{
		Super::PostLoad();
	}
};