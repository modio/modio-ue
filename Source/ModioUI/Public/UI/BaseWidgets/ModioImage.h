// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/Image.h"
#include "Core/Input/InputDeviceTypes.h"
#include "CoreMinimal.h"
#include "Misc/Optional.h"
#include "Types/ModioImageWrapper.h"
#include "UObject/StrongObjectPtr.h"

#include "ModioImage.generated.h"

/**
 *
 */
UCLASS()
class MODIOUI_API UModioImage : public UImage
{
	GENERATED_BODY()

protected:
	virtual void SynchronizeProperties();

	virtual void ImageLoadHandler(UTexture2DDynamic* Texture);
	virtual void ImageLoadHandler(UTexture2DDynamic* Texture, class UMaterialInterface* Material,
								  FName ImageParameterName);
	UPROPERTY(Transient)
	class UMaterialInstanceDynamic* CachedMaterial;

public:
	UModioImage(const FObjectInitializer& ObjectInitializer);

	virtual void SetBrushFromMaterial(UMaterialInterface* Material) override;

	UFUNCTION(BlueprintCallable, Category="ModioImage")
	virtual void DisplayImageWithMaterial(UTexture2DDynamic* Texture, UMaterialInterface* Material,
										  FName ImageParameterName);

	UFUNCTION(BlueprintCallable, Category="ModioImage")
	virtual void LoadImageFromFileAsync(FModioImageWrapper ImageLoader);

	UFUNCTION(BlueprintCallable, Category="ModioImage")
	void LoadImageFromFileWithMaterialAsync(FModioImageWrapper ImageLoader, UMaterialInterface* Material,
											FName ImageParameterName);
};
