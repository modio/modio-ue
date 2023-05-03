/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/ModioDefaultInputGlyphProvider.h"
#include "UI/Styles/ModioInputGlyphSet.h"

UModioDefaultInputGlyphProvider::UModioDefaultInputGlyphProvider()
{
	FSoftObjectPath Keyboard("/Modio/UI/DefaultKeyboardGlyphSet.DefaultKeyboardGlyphSet");

	if (Keyboard.IsValid())
	{
		KeyboardGlyphSet = Cast<UModioInputGlyphSet>(Keyboard.TryLoad());
	}

	FSoftObjectPath XBox("/Modio/UI/DefaultXBoxControllerGlyphSet.DefaultXBoxControllerGlyphSet");
	if (XBox.IsValid())
	{
		XBoxGlyphSet = Cast<UModioInputGlyphSet>(XBox.TryLoad());
	}

	FSoftObjectPath Playstation("/Modio/UI/DefaultPlaystationControllerGlyphSet.DefaultPlaystationControllerGlyphSet");
	if (Playstation.IsValid())
	{
		PlaystationGlyphSet = Cast<UModioInputGlyphSet>(Playstation.TryLoad());
	}

	FSoftObjectPath Nintendo("/Modio/UI/DefaultNintendoSwitchControllerGlyphSet.DefaultNintendoSwitchControllerGlyphSet");
	if (Nintendo.IsValid())
	{
		NintendoSwitchGlyphSet = Cast<UModioInputGlyphSet>(Nintendo.TryLoad());
	}
}

UMaterialInterface* UModioDefaultInputGlyphProvider::NativeGetInputGlyphMaterialForInputType(FKey VirtualInput, EModioUIInputMode InputType)
{
	switch (InputType)
	{
		case EModioUIInputMode::Unknown:
			return nullptr;
		case EModioUIInputMode::Mouse:
			return nullptr;
		case EModioUIInputMode::Keyboard:
			return KeyboardGlyphSet != nullptr ? KeyboardGlyphSet->GetGlyphForKey(VirtualInput) : nullptr;
		case EModioUIInputMode::XBox:
			return XBoxGlyphSet != nullptr ? XBoxGlyphSet->GetGlyphForKey(VirtualInput) : nullptr;
		case EModioUIInputMode::Playstation:
			return PlaystationGlyphSet != nullptr ? PlaystationGlyphSet->GetGlyphForKey(VirtualInput) : nullptr;
		case EModioUIInputMode::Controller:
			return XBoxGlyphSet != nullptr ? XBoxGlyphSet->GetGlyphForKey(VirtualInput) : nullptr;
		case EModioUIInputMode::NintendoSwitch:
			return NintendoSwitchGlyphSet != nullptr ? NintendoSwitchGlyphSet->GetGlyphForKey(VirtualInput) : nullptr;
		default:
			return nullptr;
	}

	return nullptr;
}
