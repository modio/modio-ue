/*
 *  Copyright (C) 2022 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#pragma once

#include "Brushes/SlateColorBrush.h"
#include "Brushes/SlateNoResource.h"
#include "Styling/SlateTypes.h"
#include "UI/Styles/ModioUIColorRef.h"
#include "UI/Styles/ModioUIStyleRef.h"
#include "UI/Styles/ModioUIWidgetStyleContainer.h"
#include "UI/Styles/ModioWidgetBorderStyle.h"

#include "ModioPopupMenuStyle.generated.h"

USTRUCT(BlueprintType)
struct MODIOUI_API FModioPopupMenuStyle : public FSlateWidgetStyle
{
	GENERATED_BODY()

	static const FName TypeName;

	virtual const FName GetTypeName() const override
	{
		return TypeName;
	};

	static const FModioPopupMenuStyle& GetDefault()
	{
		static FModioPopupMenuStyle Default;
		return Default;
	}

	FModioPopupMenuStyle() : Super() {}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Appearance)
	FSlateSound HoveredSound;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Appearance)
	FSlateSound PressedSound;
};

UCLASS(meta = (DisplayName = "ModioPopupMenuStyle"))
class UModioPopupMenuStyleContainer : public UModioUIWidgetStyleContainer
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShowOnlyInnerProperties), Category = "Widget")
	FModioPopupMenuStyle Style;

	virtual void PostLoad() override
	{
		Super::PostLoad();
	}

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return &Style;
	}
};
