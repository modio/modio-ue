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

	virtual UTexture2D* NativeGetInputGlyphTextureForInputType(FKey VirtualInput, EModioUIInputMode InputType)
		PURE_VIRTUAL(IModioUIInputHintGlyphProvider::NativeGetInputGlyphTextureForInputType, return nullptr;);

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IModioUIInputHintGlyphProvider")
	UMaterialInterface* GetInputGlyphMaterialForInputType(FKey VirtualInput, EModioUIInputMode InputType);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IModioUIInputHintGlyphProvider")
	UTexture2D* GetInputGlyphTextureForInputType(FKey VirtualInput, EModioUIInputMode InputType);

	UMaterialInterface* GetInputGlyphMaterialForInputType_Implementation(FKey VirtualInput, EModioUIInputMode InputType)
	{
		return NativeGetInputGlyphMaterialForInputType(VirtualInput, InputType);
	}

	UTexture2D* GetInputGlyphTextureForInputType_Implementation(FKey VirtualInput, EModioUIInputMode InputType)
	{
		return NativeGetInputGlyphTextureForInputType(VirtualInput, InputType);
	}
};