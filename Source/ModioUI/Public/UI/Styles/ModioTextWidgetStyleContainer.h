// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Styles/ModioUIWidgetStyleContainer.h"
#include "Styling/SlateTypes.h"

#include "ModioTextWidgetStyleContainer.generated.h"

/**
 *
 */
UCLASS(meta = (DisplayName = "ModioTextStyle"))
class MODIOUI_API UModioTextWidgetStyleContainer : public UModioUIWidgetStyleContainer
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShowOnlyInnerProperties), Category="Widget")
	FTextBlockStyle Style;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return &Style;
	}
};
