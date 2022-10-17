/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/BaseWidgets/ModioInputBindingImage.h"
#include "UI/Styles/ModioUIStyleSet.h"

void UModioInputBindingImage::OnWidgetRebuilt()
{
	Super::OnWidgetRebuilt();
	IModioUIInputDeviceChangedReceiver::Register<UModioInputBindingImage>();
}

void UModioInputBindingImage::SynchronizeProperties()
{
	Super::SynchronizeProperties();
#if WITH_EDITORONLY_DATA
	if (IsDesignTime())
	{
		UpdateBrushImage(PreviewMode);
	}
#endif
}

void UModioInputBindingImage::NativeOnInputDeviceChanged(EModioUIInputMode InputDevice)
{
	IModioUIInputDeviceChangedReceiver::NativeOnInputDeviceChanged(InputDevice);
	if (!IsDesignTime())
	{
		UpdateBrushImage(InputDevice);
		LastDeviceType = InputDevice;
	}
}

void UModioInputBindingImage::UpdateBrushImage(EModioUIInputMode InputDevice)
{
	if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		UMaterialInterface* InputGlyphMaterial = Subsystem->GetInputGlyphMaterialForInputType(KeyToShow, InputDevice);
		if (InputGlyphMaterial)
		{
			SetBrushFromMaterial(InputGlyphMaterial);
		}
	}
	if (GetVisibilityForMode.IsBound())
	{
		SetVisibility(GetVisibilityForMode.Execute(InputDevice));
		InvalidateLayoutAndVolatility();
	}
	else
	{
		// Disable input images when using Mouse/Keyboard as we do not have appropriate glyphs
		if (InputDevice == EModioUIInputMode::Keyboard || InputDevice == EModioUIInputMode::Mouse || InputDevice == EModioUIInputMode::Unknown)
		{
			SetVisibility(ESlateVisibility::Collapsed);
			InvalidateLayoutAndVolatility();
		}
	}
}

void UModioInputBindingImage::SetKeyToShow(FKey NewKey)
{
	KeyToShow = NewKey;
	UpdateBrushImage(LastDeviceType);
}

FGetVisibilityForInputModeDelegate& UModioInputBindingImage::GetInputModeVisibilityDelegate()
{
	return GetVisibilityForMode;
}
