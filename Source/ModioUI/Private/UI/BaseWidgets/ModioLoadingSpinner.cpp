// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/BaseWidgets/ModioLoadingSpinner.h"
#include "Brushes/SlateImageBrush.h"
#include "UI/BaseWidgets/Slate/SModioBox.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SBox.h"
#include "Materials/MaterialInterface.h"

const FSlateBrush* UModioLoadingSpinner::ResolveImage() const
{
	if (UMaterialInterface* Material = LoadingImage.ResolveReference())
	{
		CachedReferencedMaterial = Material;
		ImageBrush = MakeShared<FSlateImageBrush>(CachedReferencedMaterial, ImageBrushSize);
		return ImageBrush.Get();
	}
	else
	{
		CachedReferencedMaterial = nullptr;
	}
	return nullptr;
}

TSharedRef<SWidget> UModioLoadingSpinner::RebuildWidget()
{
	// clang-format off
	return SAssignNew(MyScaleBox, SScaleBox)
		.Content()
		[
			SAssignNew(MySizeBox, SBox)
			.Content()
			[
				SAssignNew(MyImage, SImage)
				.Image_UObject(this, &UModioLoadingSpinner::ResolveImage)
			]
		];
	// clang-format on
}

void UModioLoadingSpinner::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	if (MySizeBox)
	{
		MySizeBox->SetWidthOverride(ImageBrushSize.X);
		MySizeBox->SetHeightOverride(ImageBrushSize.Y);
		MyScaleBox->SetStretchDirection(StretchDirection);
		MyScaleBox->SetStretch(Stretch);
	}
}

void UModioLoadingSpinner::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	MyScaleBox.Reset();
	MySizeBox.Reset();
	MyImage.Reset();
	ImageBrush.Reset();
}
