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
#include "UI/Foundation/Base/ModioCommonActivatableWidget.h"
#include "ModioCommonDialogViewBase.generated.h"

/**
 * Dialog View Base class that allows the user to show a dialog
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonDialogViewBase : public UModioCommonActivatableWidget
{
	GENERATED_BODY()
};
