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

#include "CoreMinimal.h"
#include "Styling/SlateTypes.h"
#include "UI/Styles/ModioUIWidgetStyleContainer.h"

#include "ModioTextWidgetStyleContainer.generated.h"

/**
* Modio UI element subclass that represents the style a text can carry on
**/
UCLASS(meta = (DisplayName = "ModioTextStyle"))
class MODIOUI_API UModioTextWidgetStyleContainer : public UModioUIWidgetStyleContainer
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShowOnlyInnerProperties), Category = "Widget")
	FTextBlockStyle Style;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return &Style;
	}
};
