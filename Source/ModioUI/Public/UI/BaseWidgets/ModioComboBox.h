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

#include "Components/ComboBox.h"
#include "CoreMinimal.h"

#include "ModioComboBox.generated.h"

/**
* Modio UI element that represents a combo box, in other words
* a menu of elements with a certain placement
**/
UCLASS()
class MODIOUI_API UModioComboBox : public UComboBox
{
	GENERATED_BODY()

protected:
	virtual void SynchronizeProperties() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	TEnumAsByte<EMenuPlacement> MenuPlacement;
};
