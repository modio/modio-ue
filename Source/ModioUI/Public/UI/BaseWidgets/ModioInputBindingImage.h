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

#include "Components/SlateWrapperTypes.h"
#include "CoreMinimal.h"
#include "Delegates/DelegateCombinations.h"
#include "ModioUISubsystem.h"
#include "UI/BaseWidgets/ModioImage.h"
#include "UI/EventHandlers/IModioUIInputDeviceChangedReceiver.h"
#include "UI/Interfaces/IModioInputMappingAccessor.h"

#include "ModioInputBindingImage.generated.h"

DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(ESlateVisibility, FGetVisibilityForInputModeDelegate, EModioUIInputMode,
										 InputMode);

/**
* Class definition that links images to a binding input
**/
UCLASS()
class MODIOUI_API UModioInputBindingImage : public UModioImage,
											public IModioInputMappingAccessor,
											public IModioUIInputDeviceChangedReceiver
{
	GENERATED_BODY()

protected:
	virtual void OnWidgetRebuilt() override;
	virtual void SynchronizeProperties() override;
	virtual void NativeOnInputDeviceChanged(EModioUIInputMode InputDevice) override;
	void UpdateBrushImage(EModioUIInputMode InputDevice);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	FKey KeyToShow;

	EModioUIInputMode LastDeviceType = EModioUIInputMode::Unknown;

	FGetVisibilityForInputModeDelegate GetVisibilityForMode;

public:
#if WITH_EDITORONLY_DATA
	/**
	* Stored property to the input mode to use
	**/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	EModioUIInputMode PreviewMode;
#endif

	/**
	* Update the input key binding
	* @param NewKey Update key to store in this class
	**/
	UFUNCTION(BlueprintCallable, Category = "ModioInputBindingImage")
	void SetKeyToShow(FKey NewKey);

	/**
	* Retrieve the delegate to the input mode
	* @return The delegate used as input mode
	**/
	UFUNCTION(BlueprintCallable, Category = "ModioInputBindingImage")
	FGetVisibilityForInputModeDelegate& GetInputModeVisibilityDelegate();
};
