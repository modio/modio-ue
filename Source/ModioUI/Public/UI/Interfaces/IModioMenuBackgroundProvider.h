#pragma once

#include "Containers/Array.h"
#include "Containers/UnrealString.h"
#include "Input/Events.h"
#include "UObject/Interface.h"
#include "Materials/MaterialInterface.h"

#include "IModioMenuBackgroundProvider.generated.h"

UINTERFACE(BlueprintType)
class MODIOUI_API UModioMenuBackgroundProvider : public UInterface
{
	GENERATED_BODY()
};

class MODIOUI_API IModioMenuBackgroundProvider : public IInterface
{
	GENERATED_BODY()

protected:
	virtual UMaterialInterface* NativeGetBackgroundMaterial()
	PURE_VIRTUAL(IModioMenuBackgroundProvider::NativeGetBackgroundMaterial, return {};);

	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="ModioMenuBackgroundProvider")
	UMaterialInterface* GetBackgroundMaterial();

	UMaterialInterface* GetBackgroundMaterial_Implementation()
	{
		return NativeGetBackgroundMaterial();
	}
};
