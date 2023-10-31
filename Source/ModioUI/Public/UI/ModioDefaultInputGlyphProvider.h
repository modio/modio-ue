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

#include "Interfaces/IModioUIInputHintGlyphProvider.h"
#include "UI/Styles/ModioInputGlyphSet.h"

#include "ModioDefaultInputGlyphProvider.generated.h"

UCLASS()
class MODIOUI_API UModioDefaultInputGlyphProvider : public UObject, public IModioUIInputHintGlyphProvider
{
	GENERATED_BODY()

	UModioDefaultInputGlyphProvider();

protected:
	virtual UMaterialInterface* NativeGetInputGlyphMaterialForInputType(FKey VirtualInput, EModioUIInputMode InputType) override;
	virtual UTexture2D* NativeGetInputGlyphTextureForInputType(FKey VirtualInput, EModioUIInputMode InputType) override;

private:
	TSoftObjectPtr<UModioInputGlyphSet> KeyboardGlyphSetRef;
	TSoftObjectPtr<UModioInputGlyphSet> MacKeyboardGlyphSetRef;
	TSoftObjectPtr<UModioInputGlyphSet> XBoxGlyphSetRef;
	TSoftObjectPtr<UModioInputGlyphSet> Playstation4GlyphSetRef;
	TSoftObjectPtr<UModioInputGlyphSet> Playstation5GlyphSetRef;
	TSoftObjectPtr<UModioInputGlyphSet> NintendoSwitchGlyphSetRef;

	UModioInputGlyphSet* KeyboardGlyphSet;
	UModioInputGlyphSet* MacKeyboardGlyphSet;
	UModioInputGlyphSet* XBoxGlyphSet;
	UModioInputGlyphSet* PlaystationGlyphSet;
	UModioInputGlyphSet* NintendoSwitchGlyphSet;
};

