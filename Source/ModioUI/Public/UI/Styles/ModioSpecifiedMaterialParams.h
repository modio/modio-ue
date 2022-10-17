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
#include "UObject/SoftObjectPtr.h"

#include "ModioSpecifiedMaterialParams.generated.h"

/**
* Class definition for a Modio material parameters
**/
UCLASS(EditInlineNew)
class MODIOUI_API UModioSpecifiedMaterialParams : public UModioProceduralBrushParams
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widget")
	TSoftObjectPtr<UMaterialInterface> SpecifiedMaterial;

	virtual UMaterialInterface* GetMaterialInstance() override;
	virtual void PostLoad() override;
};
