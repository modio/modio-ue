/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#pragma once

#include "CoreMinimal.h"
#include "Objects/ModioStaticExecutionBase.h"
#include "ModioOpenWeblinkAction.generated.h"

/** Opens a weblink on the system web-browser.
 *		Requires argument containing the URL.
 */
UCLASS()
class MODIOEDITOR_API UModioOpenWeblinkAction : public UModioStaticExecutionBase
{
	GENERATED_BODY()

public:

	virtual bool ExecuteAction(FString Args) const override;
};
