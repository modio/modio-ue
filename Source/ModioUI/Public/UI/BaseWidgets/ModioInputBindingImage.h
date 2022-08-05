// Fill out your copyright notice in the Description page of Project Settings.

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
 *
 */
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

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widgets")
	FKey KeyToShow;

	EModioUIInputMode LastDeviceType = EModioUIInputMode::Unknown;

	FGetVisibilityForInputModeDelegate GetVisibilityForMode;

public:
#if WITH_EDITORONLY_DATA
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widgets")
	EModioUIInputMode PreviewMode;
#endif

	UFUNCTION(BlueprintCallable, Category="ModioInputBindingImage")
	void SetKeyToShow(FKey NewKey);
	
	UFUNCTION(BlueprintCallable, Category="ModioInputBindingImage")
	FGetVisibilityForInputModeDelegate& GetInputModeVisibilityDelegate();
};
