// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Styles/ModioNamedGlyphMaterialParams.h"
#include "Algo/Transform.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "ModioUISubsystem.h"
#include "UI/Styles/ModioUIStyleSet.h"

TArray<FString> UModioNamedGlyphMaterialParams::GetValidGlyphNames()
{
	TArray<FString> OutNames;
	if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		UModioUIStyleSet* DefaultStyleSet = Subsystem->GetDefaultStyleSet();
		if (DefaultStyleSet)
		{
			TArray<FName> GlyphNames = DefaultStyleSet->GetGlyphNames();

			Algo::Transform(GlyphNames, OutNames, [](const FName& InName) { return InName.ToString(); });
		}
	}
	return OutNames;
}

UMaterialInterface* UModioNamedGlyphMaterialParams::GetMaterialInstance()
{
	if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		UModioUIStyleSet* DefaultStyleSet = Subsystem->GetDefaultStyleSet();
		if (DefaultStyleSet)
		{
			return DefaultStyleSet->GetGlyphMaterial(GlyphName);
		}
	}
	return nullptr;
}
