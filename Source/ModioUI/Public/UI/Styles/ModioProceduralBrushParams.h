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