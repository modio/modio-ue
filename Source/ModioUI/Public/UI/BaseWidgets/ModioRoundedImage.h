// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidgets/ModioImage.h"
#include "UI/Styles/ModioUIBrushRef.h"

#include "ModioRoundedImage.generated.h"

/**
 *
 */
UCLASS()
class MODIOUI_API UModioRoundedImage : public UModioImage
{
	GENERATED_BODY()
protected:
	virtual void SynchronizeProperties() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widgets")
	FModioUIMaterialRef MaterialToUse;

	UPROPERTY(Transient)
	UMaterialInterface* CachedReferencedMaterial;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widgets")
	FName TextureParameterName = "WidgetTexture";

	virtual void LoadImageFromFileAsync(FModioImageWrapper ImageLoader) override;
};
