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
#include "Styling/SlateTypes.h"
#include "Containers/EnumAsByte.h"
#include "Framework/Views/ITypedTableView.h"
#include "ModioCommonListViewStyle.generated.h"

/**
 * The style of the List View within the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonListViewStyle : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	FTableViewStyle WidgetStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	FScrollBarStyle ScrollBarStyle;

	/** 
	 * The scroll & layout orientation of the list. ListView and TileView only. 
	 * Vertical will scroll vertically and arrange tiles into rows.
	 * Horizontal will scroll horizontally and arrange tiles into columns.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	TEnumAsByte<EOrientation> Orientation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	TEnumAsByte<ESelectionMode::Type> SelectionMode = ESelectionMode::Single;
};
