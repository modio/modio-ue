/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidgets/ModioUIAsyncLoader.h"
#include "ModioCommonUIAsyncLoader.generated.h"

/**
 * Async loader widget which can switch between three states: InProgress, Success, and Failure
 * Also automatically recursively activates and deactivates the content widget within the Common UI framework when the state changes
 */
UCLASS(Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonUIAsyncLoader : public UModioUIAsyncLoader
{
	GENERATED_BODY()

public:
	//~ Begin UModioUIAsyncLoader Interface
	virtual void NativeHandleAsyncOperationStateChange(EModioUIAsyncOperationWidgetState NewState) override;
	//~ End UModioUIAsyncLoader Interface

protected:
	/**
	 * Recursively activates or deactivates the widget and its children
	 * @param Widget Widget to activate or deactivate
	 * @param bActivate Whether to activate or deactivate the widget
	 */
	void SetActivationState_Recursive(UWidget* Widget, bool bActivate);
};
