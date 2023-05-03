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

#include "UI/Styles/ModioDialogStyle.h"
#include "UObject/Interface.h"
#include "Widgets/Input/SButton.h"

#include "IModioUIDialogButtonWidget.generated.h"

UINTERFACE(BlueprintType)
class MODIOUI_API UModioUIDialogButtonWidget : public UInterface
{
	GENERATED_BODY()
};

class MODIOUI_API IModioUIDialogButtonWidget : public IInterface
{
	GENERATED_BODY()
protected:
	virtual void NativeSetStyle(const FModioDialogStyle& Style) {}

	virtual void NativeSetDialogController(class UModioDialogController* Controller) {};

public:
	TArray<TSharedPtr<SButton>> Buttons;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ModioUIDialogButtonWidget")
	void SetStyle(const FModioDialogStyle& Style);

	void SetStyle_Implementation(const FModioDialogStyle& Style);

	/// @brief Gives the button widget a reference back to the dialog controller so that the buttons can trigger custom
	/// dialog flows
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ModioUIDialogButtonWidget")
	void SetDialogController(class UModioDialogController* Controller);

	void SetDialogController_Implementation(class UModioDialogController* Controller);
};