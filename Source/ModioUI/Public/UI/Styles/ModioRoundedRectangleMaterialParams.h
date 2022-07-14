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
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FModioUIColorRef NormalBorderColor;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FModioUIColorRef FocusedBorderColor;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FModioUIColorRef InnerColor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bRelativeRadius = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CornerRadius = 16;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float BorderThickness = 2;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bEnableBorder;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bEnableButtonGradients;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "bEnableButtonGradients", EditConditionHides))
	FModioUIColorRef BorderGradientColor;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "bEnableButtonGradients", EditConditionHides))
	FModioUIColorRef InnerGradientColor;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "bEnableButtonGradients", EditConditionHides,UIMin=0, UIMax=1))
	float BorderGradientOpacity;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "bEnableButtonGradients", EditConditionHides,UIMin=0, UIMax=1))
	float InnerGradientOpacity;

	virtual UMaterialInterface* GetMaterialInstance() override;
	#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	#endif
};
