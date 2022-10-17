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
#include "UI/BaseWidgets/ModioRetainerBox.h"
#include "UI/Styles/ModioUIStyleRef.h"

#include "ModioRoundedBorder.generated.h"

/**
* Class definition for a Modio rounded border that stores a material interface
**/
UCLASS()
class MODIOUI_API UModioRoundedBorder : public UModioRetainerBox
{
	GENERATED_BODY()
	virtual void SynchronizeProperties() override;

public:
	// TODO: make this a config var, then it can be overridden in the instances
	/**
	* Stored property reference to the material interface used in this class
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widgets")
	TSoftObjectPtr<UMaterialInterface> RetainerMaterial;

	/**
	* Stored property style reference to border type to apply
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (StyleClass = "ModioRoundedBorderStyle"), Category = "Widgets")
	FModioUIStyleRef BorderStyle;

	/*
		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Border Effect")
		bool bEnableBorder = true;

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Border Effect")
		FLinearColor BorderColor = FColor::White;

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Border Effect")
		float BorderRadius = 48;

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Border Effect")
		float BorderThickness = 4;

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Border Effect")
		float BorderOpacity = 1;*/

	/**
	* Change hover state to true
	**/
	void SetHoveredState();

	/**
	* Change hover state to false
	**/
	void ClearHoveredState();

	/**
	* Confirm that a border would be enforced
	**/
	void EnableBorder();

	/**
	 * Disable border enforcement
	 **/
	void DisableBorder();
	// BorderColour
	// radius
	// thickness
	// call GetEffectMaterial and set the parameters
};
