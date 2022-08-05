#pragma once

#include "Core/Input/InputDeviceTypes.h"
#include "InputCoreTypes.h"
#include "UObject/Interface.h"

#include "IModioUIInputHintGlyphProvider.generated.h"

UINTERFACE(BlueprintType)
class MODIOUI_API UModioUIInputHintGlyphProvider : public UInterface
{
	GENERATED_BODY()
};

class MODIOUI_API IModioUIInputHintGlyphProvider : public IInterface
{
	GENERATED_BODY()
protected:
	virtual UMaterialInterface* NativeGetInputGlyphMaterialForInputType(FKey VirtualInput, EModioUIInputMode InputType)
		PURE_VIRTUAL(IModioUIInputHintGlyphProvider::NativeGetInputGlyphMaterialForInputType, return nullptr;);

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IModioUIInputHintGlyphProvider")
	UMaterialInterface* GetInputGlyphMaterialForInputType(FKey VirtualInput, EModioUIInputMode InputType);

	UMaterialInterface* GetInputGlyphMaterialForInputType_Implementation(FKey VirtualInput, EModioUIInputMode InputType)
	{
		return NativeGetInputGlyphMaterialForInputType(VirtualInput, InputType);
	}
};