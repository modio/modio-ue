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

#include "Materials/MaterialInstanceDynamic.h"
#include "Styling/SlateTypes.h"
#include "UI/Styles/ModioUIColorRef.h"
#include "UI/Styles/ModioUIWidgetStyleContainer.h"
#include "UObject/SoftObjectPtr.h"

#include "ModioWidgetBorderStyle.generated.h"

/**
* Modio derived struct that defines the properties for a widget border style,
* which customizes color in the border, hover, or active states
**/
USTRUCT(BlueprintType)
struct MODIOUI_API FModioWidgetBorderStyle : public FSlateWidgetStyle
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
	* Retrieve a default instance of the Modio widget border style
	**/
	static const FModioWidgetBorderStyle& GetDefault()
	{
		static FModioWidgetBorderStyle Default;
		return Default;
	}

	/**
	* Stored property to enable a mask material, it defaults to true
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widgets")
	bool bMaskWithMaterial = false;

	/**
	* Stored soft pointer property to the mask material to use
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widgets")
	TSoftObjectPtr<UMaterialInterface> MaskMaterial;

	/// @brief Corner Radius will be multiplied against the shortest dimension of the widget instead of being absolute
	/// in pixel terms
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Border Effect")
	bool bRelativeRadius = false;

	/**
	* Stored property that represents the corner radius, by default is 48
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Border Effect")
	float CornerRadius = 48;

	/**
	* Stored property to enable a border, it defaults to true
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Border Effect")
	bool bEnableBorder = true;

	/**
	* Stored property of the border UI color reference
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Border Effect")
	FModioUIColorRef BorderColor;

	/**
	* Stored property of the hover UI color reference
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Border Effect")
	FModioUIColorRef HoverColor;

	/**
	* Stored property of the active UI color reference
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Border Effect")
	FModioUIColorRef ActiveColor;

	/**
	* Stored property of the inner UI color reference
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Border Effect")
	FModioUIColorRef NewInnerColor;

	/**
	* Stored property about the border thickness, with default as 4
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Border Effect")
	float BorderThickness = 4;

	/**
	* Stored property that represents the border opacity, with default as 1
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Border Effect")
	float BorderOpacity = 1;

	/**
	* Stored property boolean to either use a widget texture or not
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inner Content")
	bool bUseWidgetTexture = 1;

	/**
	* Stored property that represents the inner color with 'white' as default
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inner Content")
	FLinearColor InnerColor = FColor::White;
};

/**
* Modify the border style to a Material instance
* @param WidgetMaterial The material instance that will change
* @param NewStyle The update border style to use
**/
void ApplyModioBorderStyle(UMaterialInstanceDynamic* WidgetMaterial, const FModioWidgetBorderStyle* NewStyle);

/**
* Modio element to wrap a FModioWidgetBorderStyle using UModioUIWidgetStyleContainer properties
**/
UCLASS(meta = (DisplayName = "ModioRoundedBorderStyle"))
class UModioRoundedBorderStyle : public UModioUIWidgetStyleContainer
{
	GENERATED_BODY()
public:
	/**
	* Stored property to the border style to use in this class
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShowOnlyInnerProperties), Category = "Widgets")
	FModioWidgetBorderStyle Style;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return &Style;
	}
};