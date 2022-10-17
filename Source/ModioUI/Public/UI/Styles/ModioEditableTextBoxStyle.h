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

#include "Misc/EngineVersionComparison.h"
#include "Styling/SlateTypes.h"
#include "UI/Styles/ModioUIColorRef.h"
#include "UI/Styles/ModioUIWidgetStyleContainer.h"

#include "ModioEditableTextBoxStyle.generated.h"

/**
* Modio derived struct that defines the properties for a editable text box,
* with slate brush properties according to their normal, error or hover state
**/
USTRUCT(BlueprintType)
struct MODIOUI_API FModioEditableTextBoxStyle : public FEditableTextBoxStyle
{
	GENERATED_BODY()
	
    /**
    * Static stored property of the type name
    **/
	static const FName TypeName;

    /**
    * Function to retrieve the type name stored property
    * @return FName with the type
    **/
	virtual const FName GetTypeName() const override
	{
		return TypeName;
	};
	
	/**
	* Stored property to conditionally show the hint icon or not
	**/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (InlineEditConditionToggle), Category = "Widget")
	bool bShowHintIcon = true;

	/**
	* Stored property to represent the hint brush
	**/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (EditCondition = bShowHintIcon), Category = "Widget")
	FSlateBrush HintIcon;

	/**
	* Stored property to represent the normal border brush
	**/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Border")
	FSlateBrush NormalBorderBrush;

	/**
	* Stored property to represent the hovered border brush
	**/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Border")
	FSlateBrush HoveredBorderBrush;

	/**
	* Stored property to represent the normal border brush
	**/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Border")
	FSlateBrush ErrorBorderBrush;

	/**
	* Retrieve a default instance of the editable text box 
	**/
	static const FModioEditableTextBoxStyle& GetDefault()
	{
		static FModioEditableTextBoxStyle Default;
		return Default;
	}
};

/**
* Modio element to wrap a FModioEditableTextBoxStyle using UModioUIWidgetStyleContainer properties
**/
UCLASS(meta = (DisplayName = "ModioEditableTextBoxStyle"))
class UModioEditableTextBoxStyleContainer : public UModioUIWidgetStyleContainer
{
	GENERATED_BODY()
public:
	/**
	* Stored property to represent the container style
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShowOnlyInnerProperties), Category = "Widget")
	FModioEditableTextBoxStyle Style;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return &Style;
	}
	virtual void PostLoad() override
	{
		UModioUIWidgetStyleContainer::PostLoad();
#if UE_VERSION_NEWER_THAN(5, 0, 0)
		Style.FocusedForegroundColor = Style.ForegroundColor;
#endif
	}
};