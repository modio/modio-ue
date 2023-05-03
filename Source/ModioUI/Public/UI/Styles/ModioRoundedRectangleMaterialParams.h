/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#pragma once

#include "Materials/MaterialInterface.h"
#include "UI/Styles/ModioProceduralBrushParams.h"
#include "UI/Styles/ModioUIColorRef.h"

#include "ModioRoundedRectangleMaterialParams.generated.h"

/**
* Class definition for the material parameters of a rounded rectangle 
**/
UCLASS(EditInlineNew)
class MODIOUI_API UModioRoundedRectangleMaterialParams : public UModioProceduralBrushParams
{
	GENERATED_BODY()
public:
	/**
	* Stored property of the border color to use with normal behaviour
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	FModioUIColorRef NormalBorderColor;
	
	/**
	* Stored property of the border color to use with focused behaviour
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	FModioUIColorRef FocusedBorderColor;
	
	/**
	* Stored property of the border color to use as inner color
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	FModioUIColorRef InnerColor;

	/**
	* Stored property boolean to use a relative radius, false by default
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	bool bRelativeRadius = false;

	/**
	* Stored property to the corner radius to use. By default it is 16 points
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	float CornerRadius = 16;
	
	/**
	* Stored property to the border thickness to use. By default it is 2 points
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	float BorderThickness = 2;
	
	/**
	* Stored property boolean to enable a border around this widget instance
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	bool bEnableBorder;

	/**
	* Stored property boolean to enable a gradient on the reference button
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	bool bEnableButtonGradients;
	
	/**
	* Stored property of the color gradient to use at the border of the reference button
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "bEnableButtonGradients", EditConditionHides),
			  Category = "Widget")
	FModioUIColorRef BorderGradientColor;
	
	/**
	* Stored property of the reference button inner color gradient
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "bEnableButtonGradients", EditConditionHides),
			  Category = "Widget")
	FModioUIColorRef InnerGradientColor;
	
	/**
	* Stored property of the gradient opacity applied to the border
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere,
			  meta = (EditCondition = "bEnableButtonGradients", EditConditionHides, UIMin = 0, UIMax = 1),
			  Category = "Widget")
	float BorderGradientOpacity;
	
	/**
	* Stored property of the inner gradient opacity applied to the widget
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere,
			  meta = (EditCondition = "bEnableButtonGradients", EditConditionHides, UIMin = 0, UIMax = 1),
			  Category = "Widget")
	float InnerGradientOpacity;

	virtual UMaterialInterface* GetMaterialInstance() override;


	void ModifyMaterial(UMaterialInstanceDynamic* MaterialToModify);
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
