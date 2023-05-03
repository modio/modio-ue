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

#include "CoreMinimal.h"
#include "UI/Styles/ModioProceduralBrushParams.h"
#include "UI/Styles/ModioUIColorRef.h"
#include "ModioCheckBoxMaterialParams.generated.h"

/**
 * 
 */
UCLASS()
class MODIOUI_API UModioCheckBoxMaterialParams : public UModioProceduralBrushParams
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Modio Checkbox params")
	FModioUIColorRef InnerColor;

	UPROPERTY(EditAnywhere, Category = "Modio Checkbox params")
	FModioUIColorRef TickColor;

	UPROPERTY(EditAnywhere, Category = "Modio Checkbox params")
	FModioUIColorRef BorderColor;

	UPROPERTY(EditAnywhere, Category = "Modio Checkbox params")
	bool bIsTicked = false;

	UPROPERTY(EditAnywhere, Category = "Modio Checkbox params")
	bool bEnableBorder = false;

	UPROPERTY(EditAnywhere, Category = "Modio Checkbox params")
	float BorderRadius = 0.5;

	virtual UMaterialInterface* GetMaterialInstance() override;

	void ModifyMaterial(UMaterialInstanceDynamic* MaterialToModify);
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
