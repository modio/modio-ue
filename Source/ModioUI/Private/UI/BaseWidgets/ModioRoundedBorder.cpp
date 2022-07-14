// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/BaseWidgets/ModioRoundedBorder.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Slate/SRetainerWidget.h"
#include "UI/Styles/ModioWidgetBorderStyle.h"

void UModioRoundedBorder::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	if (RetainerMaterial.IsValid())
	{
		SetEffectMaterial(RetainerMaterial.Get());
	}
	UMaterialInstanceDynamic* CurrentMaterial = GetEffectMaterial();
	/*if (!CurrentMaterial) {
		SetEffectMaterial(RetainerMaterial.Get());
		CurrentMaterial = GetEffectMaterial();
	}*/

	const FModioWidgetBorderStyle* ResolvedStyle = BorderStyle.FindStyle<FModioWidgetBorderStyle>();
	ApplyModioBorderStyle(CurrentMaterial, ResolvedStyle);
	/*
		if (CurrentMaterial)
		{
			CurrentMaterial->SetScalarParameterValue(FName("EnableBorder"), bEnableBorder ? 1 : 0);
			CurrentMaterial->SetScalarParameterValue(FName("BorderOpacity"), BorderOpacity);
			CurrentMaterial->SetScalarParameterValue(FName("Thickness"), BorderThickness);
			CurrentMaterial->SetScalarParameterValue(FName("Radius"), BorderRadius);
			CurrentMaterial->SetVectorParameterValue(FName("Color"), BorderColor);
		}
	*/

	MyRetainerWidget->SetRetainedRendering(true);
}

void UModioRoundedBorder::SetHoveredState()
{
	if (!GetEffectMaterial()) {
		if (RetainerMaterial.IsValid())
		{
			SetEffectMaterial(RetainerMaterial.Get());
		}
	}
	if (GetEffectMaterial())
	{
		GetEffectMaterial()->SetScalarParameterValue(FName("Hovered"), 1);
	}
}

void UModioRoundedBorder::ClearHoveredState()
{
	GetEffectMaterial()->SetScalarParameterValue(FName("Hovered"), 0);
}

void UModioRoundedBorder::EnableBorder()
{
	GetEffectMaterial()->SetScalarParameterValue(FName("EnableBorder"), 1);
}

void UModioRoundedBorder::DisableBorder()
{
	GetEffectMaterial()->SetScalarParameterValue(FName("EnableBorder"), 0);
}
