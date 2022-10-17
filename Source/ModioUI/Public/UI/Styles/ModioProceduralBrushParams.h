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

#include "ModioProceduralBrushParams.generated.h"

UCLASS(EditInlineNew, abstract, collapseCategories)
class MODIOUI_API UModioProceduralBrushParams : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION()
	virtual UMaterialInterface* GetMaterialInstance()
		PURE_VIRTUAL(UModioProceduralBrushParams::GetMaterialInstance, return nullptr;);
};