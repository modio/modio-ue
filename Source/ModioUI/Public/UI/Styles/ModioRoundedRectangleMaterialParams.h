#pragma once

#include "Materials/MaterialInterface.h"
#include "UI/Styles/ModioProceduralBrushParams.h"
#include "UI/Styles/ModioUIColorRef.h"

#include "ModioRoundedRectangleMaterialParams.generated.h"

UCLASS(EditInlineNew)
class MODIOUI_API UModioRoundedRectangleMaterialParams : public UModioProceduralBrushParams
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widget")
	FModioUIColorRef NormalBorderColor;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widget")
	FModioUIColorRef FocusedBorderColor;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widget")
	FModioUIColorRef InnerColor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widget")
	bool bRelativeRadius = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widget")
	float CornerRadius = 16;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widget")
	float BorderThickness = 2;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widget")
	bool bEnableBorder;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widget")
	bool bEnableButtonGradients;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "bEnableButtonGradients", EditConditionHides), Category="Widget")
	FModioUIColorRef BorderGradientColor;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "bEnableButtonGradients", EditConditionHides), Category="Widget")
	FModioUIColorRef InnerGradientColor;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "bEnableButtonGradients", EditConditionHides,UIMin=0, UIMax=1), Category="Widget")
	float BorderGradientOpacity;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "bEnableButtonGradients", EditConditionHides,UIMin=0, UIMax=1), Category="Widget")
	float InnerGradientOpacity;

	virtual UMaterialInterface* GetMaterialInstance() override;
	#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	#endif
};
