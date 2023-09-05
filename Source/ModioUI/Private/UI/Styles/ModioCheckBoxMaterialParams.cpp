/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/Styles/ModioCheckBoxMaterialParams.h"

#include "ModioUI4Subsystem.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "ModioUISubsystem.h"
#include "UI/Styles/ModioUIStyleSet.h"

UMaterialInterface* UModioCheckBoxMaterialParams::GetMaterialInstance()
{
	if (UModioUI4Subsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUI4Subsystem>())
	{
		UModioUIStyleSet* DefaultStyleSet = Subsystem->GetDefaultStyleSet();

		if (DefaultStyleSet)
		{
			UMaterialInstanceDynamic* material = UMaterialInstanceDynamic::Create(DefaultStyleSet->GetDefaultCheckboxMaterial(), NULL);
			if (material)
			{
				ModifyMaterial(material);
				return material;
			}
		}
	}
	return nullptr;
}

void UModioCheckBoxMaterialParams::ModifyMaterial(UMaterialInstanceDynamic* MaterialToModify) 
{
	MaterialToModify->SetScalarParameterValue(FName("EnableBorder"), bEnableBorder ? 1 : 0);
	MaterialToModify->SetScalarParameterValue(FName("BorderOpacity"), 1);
	MaterialToModify->SetScalarParameterValue(FName("ShowTick"), bIsTicked ? 1 : 0);
	MaterialToModify->SetScalarParameterValue(FName("Radius"), BorderRadius);
	MaterialToModify->SetScalarParameterValue(FName("UseRenderTarget"), 0);
	MaterialToModify->SetVectorParameterValue(FName("InnerColor"), InnerColor.ResolveReference().GetSpecifiedColor());
	MaterialToModify->SetVectorParameterValue(FName("Color"), BorderColor.ResolveReference().GetSpecifiedColor());
	MaterialToModify->SetVectorParameterValue(FName("TickColor"), TickColor.ResolveReference().GetSpecifiedColor());
}

#if WITH_EDITOR
void UModioCheckBoxMaterialParams::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {}
#endif
