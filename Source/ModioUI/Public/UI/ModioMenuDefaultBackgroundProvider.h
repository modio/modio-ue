#pragma once

#include "Interfaces/IModioMenuBackgroundProvider.h"

#include "ModioMenuDefaultBackgroundProvider.generated.h"

UCLASS()
class MODIOUI_API UModioMenuDefaultBackgroundProvider : public UObject, public IModioMenuBackgroundProvider
{
	GENERATED_BODY()

protected:
	UMaterialInterface* NativeGetBackgroundMaterial();

private:
	UMaterialInterface* BackgroundMaterial;
};
