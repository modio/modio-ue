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
#include "ModioSelectContentAction.generated.h"

/** An action that selects content in the content browser, navigating to it if needed.
 *		Requires argument for the content to select. Theoretically can select multiple.
 */
UCLASS()
class MODIOEDITOR_API UModioSelectContentAction : public UModioStaticExecutionBase
{
	GENERATED_BODY()

public:

	virtual bool ExecuteAction(FString Args) const override;
	
};
