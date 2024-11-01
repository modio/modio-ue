/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "CoreMinimal.h"
#include "Objects/ModioStaticExecutionBase.h"
#include "ModioOpenEditorWidgetAction.generated.h"

/** Opens an Editor Utility Widget
 *		Requres argument of the path to the widget to open.
 */
UCLASS()
class MODIOEDITOR_API UModioOpenEditorWidgetAction : public UModioStaticExecutionBase
{
	GENERATED_BODY()

	virtual bool ExecuteAction(FString Args) const override;
	
};
