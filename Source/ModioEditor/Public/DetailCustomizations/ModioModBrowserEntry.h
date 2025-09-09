/*
 *  Copyright (C) 2025 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "CoreMinimal.h"
#include "Engine/Texture2DDynamic.h"
#include "ModioSubsystem.h"
#include "Types/ModioModInfo.h"
#include "Widgets/SCompoundWidget.h"

/**
 *
 */
class MODIOEDITOR_API SModioModBrowserEntry : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SModioModBrowserEntry) {}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs, TSharedPtr<FModioModInfo>& InModInfo, UModioSubsystem* InModioSubsystem);

private:
	UModioSubsystem* ModioSubsystem;

	TSharedPtr<FModioModInfo> ModInfo;
	TSharedPtr<SBox> ImageBox;

	TSharedPtr<FSlateImageBrush> ThumbnailImageBrush;

	void DrawThumbnailImage();
	void OnImageDownloaded(FModioErrorCode ErrorCode, TOptional<FModioImageWrapper> Image);
	void OnImageLoaded(UTexture2DDynamic* LoadedImage);

	const FString GetStatusStringFromEnum(EModioModServerSideStatus StatusEnum) const;
	const FString GetVisibilityStringFromEnum(EModioObjectVisibilityFlags VisibilityFlags) const;
};
