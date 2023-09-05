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

#include "ModioUI4Subsystem.h"
#include "Engine/Engine.h"
#include "ModioUISubsystem.h"
#include "Styling/ISlateStyle.h"
#include "UI/Styles/ModioUIStyleSet.h"
#include "UObject/NameTypes.h"

#include "ModioUIStyleRef.generated.h"

/**
* Struct definition for the UI style reference, which stores a name and 
* a style type
**/
USTRUCT(BlueprintType)
struct FModioUIStyleRef
{
	GENERATED_BODY()
protected:
	/**
	* Stored property of the style name
	**/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Style")
	FName StyleName;

public:
	/**
	* Default constructor without parameters
	**/
	FModioUIStyleRef() : StyleName(NAME_None) {};
	
	/**
	* Convenience constructor with a style name parameter
	* @param StyleName Name of the style to store in this instance
	**/
	FModioUIStyleRef(FName StyleName) : StyleName(StyleName) {};

	// TODO: Implement a custom K2Node for resolving the style
	/**
	* Query the engine about the style name stored and retrieve the style type
	* @return StyleType when the name is found, otherwise nullptr
	**/
	template<typename StyleType>
	const StyleType* const FindStyle() const
	{
		UModioUI4Subsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUI4Subsystem>();
		if (Subsystem)
		{
			UModioUIStyleSet* DefaultStyle = Subsystem->GetDefaultStyleSet();
			if (DefaultStyle && DefaultStyle->HasWidgetStyle<StyleType>(StyleName))
			{
				DefaultStyle->RefreshStyleMaterials(StyleName);
				return &DefaultStyle->GetWidgetStyle<StyleType>(StyleName);
			}
		}
		return nullptr;
	}

	/**
	* Update the style name stored
	* @param NewName The update to set the stored style name
	**/
	void SetStyleName(FName NewName)
	{
		StyleName = NewName;
	}

	/**
	* Retrieve the style name stored
	* @return FName stored in this instance
	**/
	FName GetStyleName() const
	{
		return StyleName;
	}
};