/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/BaseWidgets/ModioLoadingSpinner.h"
#include "Brushes/SlateImageBrush.h"
#include "Materials/MaterialInterface.h"
#include "UI/BaseWidgets/Slate/SModioBox.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SBox.h"

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
