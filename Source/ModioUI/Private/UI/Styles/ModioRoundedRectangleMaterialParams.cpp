/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/Styles/ModioRoundedRectangleMaterialParams.h"

#include "ModioUI4Subsystem.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "ModioUISubsystem.h"
#include "UI/Styles/ModioUIStyleSet.h"

UMaterialInterface* UModioRoundedRectangleMaterialParams::GetMaterialInstance()
{

	if (UModioUI4Subsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUI4Subsystem>())
	{
		UModioUIStyleSet* DefaultStyleSet = Subsystem->GetDefaultStyleSet();

		if (DefaultStyleSet)
		{
			UMaterialInterface* roundedRectangleMaterial = DefaultStyleSet->GetDefaultRoundedRectangleMaterial();
			if (!roundedRectangleMaterial)
			{
				return nullptr;
			}

			UMaterialInstanceDynamic* material = UMaterialInstanceDynamic::Create(roundedRectangleMaterial, NULL);
			if (IsValid(material))
			{
				ModifyMaterial(material);
			}
			return material;
			
		}
	}
	return nullptr;
}
void UModioRoundedRectangleMaterialParams::ModifyMaterial(UMaterialInstanceDynamic* MaterialToModify) 
{
		MaterialToModify->SetScalarParameterValue(FName("RelativeRadius"), bRelativeRadius ? 1 : 0);
		MaterialToModify->SetScalarParameterValue(FName("EnableBorder"), bEnableBorder ? 1 : 0);
		MaterialToModify->SetScalarParameterValue(FName("BorderOpacity"), 1);
		MaterialToModify->SetScalarParameterValue(FName("Thickness"), BorderThickness);
		MaterialToModify->SetScalarParameterValue(FName("Radius"), CornerRadius);
		MaterialToModify->SetVectorParameterValue(FName("Color"), NormalBorderColor.ResolveReference().GetSpecifiedColor());
		MaterialToModify->SetVectorParameterValue(FName("HoverColor"), FocusedBorderColor.ResolveReference().GetSpecifiedColor());
		MaterialToModify->SetScalarParameterValue(FName("Hovered"), 0);
		MaterialToModify->SetVectorParameterValue(FName("ActiveColor"), FocusedBorderColor.ResolveReference().GetSpecifiedColor());
		MaterialToModify->SetScalarParameterValue(FName("UseRenderTarget"), 0);
		MaterialToModify->SetVectorParameterValue(FName("InnerColor"), InnerColor.ResolveReference().GetSpecifiedColor());
		MaterialToModify->SetScalarParameterValue(FName("EnableButtonGradients"), bEnableButtonGradients ? 1 : 0);
		MaterialToModify->SetScalarParameterValue(FName("BorderGradientOpacity"), BorderGradientOpacity);
		MaterialToModify->SetScalarParameterValue(FName("InnerGradientOpacity"), InnerGradientOpacity);
		MaterialToModify->SetVectorParameterValue(FName("BorderGradientColor"), BorderGradientColor.ResolveReference().GetSpecifiedColor());
		MaterialToModify->SetVectorParameterValue(FName("InnerGradientColor"), InnerGradientColor.ResolveReference().GetSpecifiedColor());
}
#if WITH_EDITOR
void UModioRoundedRectangleMaterialParams::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
