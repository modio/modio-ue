#pragma once

#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "SlateMaterialBrush.h"

#include "ModioSlateMaterialBrush.generated.h"

USTRUCT(BlueprintType)
struct FModioSlateMaterialBrush : public FSlateBrush
{
	GENERATED_BODY()

	FModioSlateMaterialBrush() = default;
	~FModioSlateMaterialBrush() = default;
	FModioSlateMaterialBrush(UMaterialInterface* InMaterial, const FVector2D& InMaterialSize)
		: FSlateBrush(ESlateBrushDrawType::Image, FName(InMaterial->GetFName()), FMargin(0),
					  ESlateBrushTileType::NoTile, ESlateBrushImageType::FullColor, InMaterialSize, FLinearColor::White)
	{
		SetResourceObject(InMaterial);
		// MaterialInstance->AddToRoot();
	}
};