// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/Widget.h"
#include "CoreMinimal.h"
#include "UI/Styles/ModioUIBrushRef.h"
#include "Widgets/Layout/SScaleBox.h"

#include "ModioLoadingSpinner.generated.h"

/**
 *
 */
UCLASS()
class MODIOUI_API UModioLoadingSpinner : public UWidget
{
	GENERATED_BODY()
protected:
	TSharedPtr<class SScaleBox> MyScaleBox;
	TSharedPtr<class SBox> MySizeBox;
	TSharedPtr<class SImage> MyImage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widgets")
	FModioUIMaterialRef LoadingImage = FModioUIMaterialRef {"Loading"};
	
	UPROPERTY(BlueprintReadOnly,EditAnywhere, Category="Widgets")
	FVector2D ImageBrushSize = FVector2D(32,32);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stretching")
	TEnumAsByte<EStretch::Type> Stretch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stretching")
	TEnumAsByte<EStretchDirection::Type> StretchDirection;

	mutable TSharedPtr<struct FSlateImageBrush> ImageBrush;

	UPROPERTY(Transient)
	mutable UMaterialInterface* CachedReferencedMaterial;


	virtual const FSlateBrush* ResolveImage() const;

	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void SynchronizeProperties() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
};
