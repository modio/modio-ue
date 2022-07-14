// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidgets/ModioRoundedImage.h"
#include "UI/BaseWidgets/ModioUserWidgetBase.h"
#include "UI/Interfaces/IModioUIImageDisplay.h"
#include "UI/Styles/ModioUIBrushRef.h"

#include "ModioModDetailsImage.generated.h"

/**
 *
 */
UCLASS()
class MODIOUI_API UModioModDetailsImage : public UModioUserWidgetBase, public IModioUIImageDisplay
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (BindWidget))
	UModioRoundedImage* Image;

	void NativeDisplayImage(const FSlateBrush& InImage);

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FModioUIMaterialRef FrameMaterial;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FName TextureParameterName = "WidgetTexture";
};
