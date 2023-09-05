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

#include "ModioUI4Subsystem.h"
#include "UI/Styles/ModioUIStyleSet.h"
#include "Settings/ModioUISettings.h"

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
		if (KeyToShow.IsValid() && KeyToShow.GetFName() != NAME_None)
		{
			UpdateBrushImage(InputDevice);
			LastDeviceType = InputDevice;
		}
	}
}

void UModioInputBindingImage::UpdateBrushImage(EModioUIInputMode InputDevice)
{
	UModioUISettings* Settings = UModioUISettings::StaticClass()->GetDefaultObject<UModioUISettings>();
	// Disable input images when bDisableInputGlyphsCompletly is set true
	if (Settings->bDisableInputGlyphsCompletely)
	{
		SetVisibility(ESlateVisibility::Collapsed);
		InvalidateLayoutAndVolatility();
		return;
  }
  
	if (UModioUI4Subsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUI4Subsystem>())
	{
		UMaterialInterface* InputGlyphMaterial = Subsystem->GetInputGlyphMaterialForInputType(KeyToShow, InputDevice);
		if (IsValid(InputGlyphMaterial))
		{
			SetBrushFromMaterial(InputGlyphMaterial);
			SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		} else
		{
			SetVisibility(ESlateVisibility::Collapsed);
			InvalidateLayoutAndVolatility();
		}
	}
	if (GetVisibilityForMode.IsBound())
	{
		SetVisibility(GetVisibilityForMode.Execute(InputDevice));
		InvalidateLayoutAndVolatility();
	}
	else
	{
		// Disable input images when input device is unknown
		if (InputDevice == EModioUIInputMode::Unknown)
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
