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
#include "UI/Styles/ModioUIBrushRef.h"
#include "Widgets/Layout/SScaleBox.h"

#include "ModioLoadingSpinner.generated.h"

/**
* Base class that represents a Modio loading spinner
**/
UCLASS()
class MODIOUI_API UModioLoadingSpinner : public UWidget
{
	GENERATED_BODY()
protected:
	TSharedPtr<class SScaleBox> MyScaleBox;
	TSharedPtr<class SBox> MySizeBox;
	TSharedPtr<class SImage> MyImage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	FModioUIMaterialRef LoadingImage = FModioUIMaterialRef {"Loading"};

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	FVector2D ImageBrushSize = FVector2D(32, 32);

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
