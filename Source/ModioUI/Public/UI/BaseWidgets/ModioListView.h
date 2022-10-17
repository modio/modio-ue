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

#include "Components/ListView.h"
#include "CoreMinimal.h"

#include "ModioListView.generated.h"

/**
* Modio UI element that displays widgets as a list
**/
UCLASS()
class MODIOUI_API UModioListView : public UListView
{
	GENERATED_BODY()
protected:
#if WITH_EDITORONLY_DATA
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	bool bSimulateSelection = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	int32 SimulatedSelectionIndex = 0;
#endif

	virtual void SynchronizeProperties() override;
};
