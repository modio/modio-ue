#pragma once
#include "Materials/MaterialInstanceDynamic.h"
#include "Styling/SlateTypes.h"
#include "UI/Styles/ModioUIColorRef.h"
#include "UI/Styles/ModioUIWidgetStyleContainer.h"
#include "UObject/SoftObjectPtr.h"

#include "ModioWidgetBorderStyle.generated.h"

USTRUCT(BlueprintType)
struct MODIOUI_API FModioWidgetBorderStyle : public FSlateWidgetStyle
{
	GENERATED_BODY()
	static const FName TypeName;
	virtual const FName GetTypeName() const override
	{
		return TypeName;
	};

	static const FModioWidgetBorderStyle& GetDefault()
	{
		static FModioWidgetBorderStyle Default;
		return Default;
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widgets")
	bool bMaskWithMaterial = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widgets")
	TSoftObjectPtr<UMaterialInterface> MaskMaterial;

	/// @brief Corner Radius will be multiplied against the shortest dimension of the widget instead of being absolute
	/// in pixel terms
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Border Effect")
	bool bRelativeRadius = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Border Effect")
	float CornerRadius = 48;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Border Effect")
	bool bEnableBorder = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Border Effect")
	FModioUIColorRef BorderColor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Border Effect")
	FModioUIColorRef HoverColor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Border Effect")
	FModioUIColorRef ActiveColor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Border Effect")
	FModioUIColorRef NewInnerColor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Border Effect")
	float BorderThickness = 4;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Border Effect")
	float BorderOpacity = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inner Content")
	bool bUseWidgetTexture = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inner Content")
	FLinearColor InnerColor = FColor::White;
};

void ApplyModioBorderStyle(UMaterialInstanceDynamic* WidgetMaterial, const FModioWidgetBorderStyle* NewStyle);

UCLASS(meta = (DisplayName = "ModioRoundedBorderStyle"))
class UModioRoundedBorderStyle : public UModioUIWidgetStyleContainer
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShowOnlyInnerProperties), Category="Widgets")
	FModioWidgetBorderStyle Style;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return &Style;
	}
};