// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Styles/ModioSpecifiedMaterialParams.h"
#include "Materials/MaterialInstance.h"

UMaterialInterface* UModioSpecifiedMaterialParams::GetMaterialInstance() 
{
	if (!SpecifiedMaterial.IsNull())
	{
		if (UMaterialInterface* LoadedMaterial = SpecifiedMaterial.LoadSynchronous())
		{
			return LoadedMaterial;
		}
	}
	return nullptr;
}

void UModioSpecifiedMaterialParams::PostLoad() 
{
	Super::PostLoad();
	//GetMaterialInstance();
}
