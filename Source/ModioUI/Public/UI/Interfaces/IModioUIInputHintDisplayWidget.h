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

#include "InputCoreTypes.h"
#include "Types/ModioCommonTypes.h"
#include "Types/ModioUser.h"
#include "UObject/Interface.h"

#include "IModioUIInputHintDisplayWidget.generated.h"

UINTERFACE(BlueprintType)
class MODIOUI_API UModioUIInputHintDisplayWidget : public UInterface
{
	GENERATED_BODY()
};

class MODIOUI_API IModioUIInputHintDisplayWidget : public IInterface
{
	GENERATED_BODY()

	bool bRoutedDisplayHintForInput = false;

protected:
	virtual void NativeDisplayHintForInput(FKey VirtualKey);

	UFUNCTION(BlueprintImplementableEvent)
	void OnDisplayHintForInput(FKey VirtualKey);

public:
	void DisplayHintForInputHandler(FKey VirtualKey);
};
