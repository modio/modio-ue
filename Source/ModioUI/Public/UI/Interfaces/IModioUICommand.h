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

#include "UObject/Interface.h"

#include "IModioUICommand.generated.h"

UINTERFACE(BlueprintType)
class MODIOUI_API UModioUICommand : public UInterface
{
	GENERATED_BODY()
};

class MODIOUI_API IModioUICommand : public IInterface
{
	GENERATED_BODY()
protected:
	// virtual TSHaredPtr<FUICommandList> GetUICommands // implemented in interface
	// virtual TSharedPtr<FUICommandList> AddCommands; // implemented in derived classes

public:
	// virtual FReply HandleCommands(FKeyEvent Event) // virtual so can be overridden but default implementation in
	// interface?
};