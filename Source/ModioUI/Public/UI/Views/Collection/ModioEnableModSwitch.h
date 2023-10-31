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

#include "CoreMinimal.h"
#include "UI/Styles/ModioButtonStyle.h"
#include "UI/Styles/ModioUIStyleRef.h"
#include "UI/BaseWidgets/ModioUserWidgetBase.h"
#include "ModioEnableModSwitch.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnModEnableButtonPressed);

UCLASS()
class MODIOUI_API UModioEnableModSwitch : public UModioUserWidgetBase
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	class UModioButton* EnableModButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* SwitchButton;

	UPROPERTY(meta = (BindWidget))
	class UModioRoundedImage* BackgroundImage;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* EnableModAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* ModErrorAnimation;

	UPROPERTY(meta = (BindWidget))
	class UScaleBox* InputBindingScaleBox;

	UPROPERTY(meta = (BindWidget))
	class USizeBox* ButtonSizeBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widgets")
	FModioUIStyleRef EnabledStyle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widgets")
	FModioUIStyleRef DisabledStyle;

	// The list elements will get scrambled after updating so this is used to offset the button location manually on initialization. 
	// Please set this as the same as in the EnableModAnimation end frame
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	FVector2D ButtonInitTransform = FVector2D(12.0f, 0.0f);

	FOnModEnableButtonPressed OnButtonPressed;

	UPROPERTY()
	bool bIsModEnabled = false;

	bool bErrorState = true;

	bool bUpdateTriggered = false;

	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void TryToggleButton();

	UFUNCTION()
	void InitWithModState(bool bIsInactive = true, bool bEnabled = false);

	UFUNCTION()
	void SetButtonState(bool bEnabled);

	UFUNCTION()
	void OnButtonHovered();

	UFUNCTION()
	void OnButtonUnhovered();





};
