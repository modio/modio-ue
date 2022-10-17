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

#include "Components/Widget.h"
#include "CoreMinimal.h"

#include "ModioCircle.generated.h"

class SModioCircle;

/**
* Modio UI element that that represent a Widget circle 
**/
UCLASS()
class MODIOUI_API UModioCircle : public UWidget
{
	GENERATED_BODY()
protected:
	TSharedRef<SWidget> RebuildWidget() override;
	TSharedPtr<SModioCircle> MyCircle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = 10, ClampMax = 1000), Category = "Widgets")
	float Resolution = 60;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = 0.0, ClampMax = 360.0), Category = "Widgets")
	float StartAngleInDegrees = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = 0.0, ClampMax = 360.0), Category = "Widgets")
	float ArcInDegrees = 360;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = 1), Category = "Widgets")
	float Radius = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = 1), Category = "Widgets")
	float Thickness = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widgets")
	FSlateColor ColorAndOpacity = FLinearColor::White;

public:
	/**
	*  Called by the editor to update modified state
	**/
	void SynchronizeProperties() override;

	/**
	* Clear resources in this Widget circle
	* @param bReleaseChildren Trigger release as well for super classes
	**/
	void ReleaseSlateResources(bool bReleaseChildren) override;
};
