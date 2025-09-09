/*
 *  Copyright (C) 2025 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "DetailCustomizations/ModioModBrowserEntry.h"

#include "SlateOptMacros.h"
#include "Types/ModioErrorCode.h"
#include "Types/ModioImageWrapper.h"
#include "Widgets/Images/SThrobber.h"
#include "Widgets/SBoxPanel.h"

#define LOCTEXT_NAMESPACE "ModBrowserEntry"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SModioModBrowserEntry::Construct(const FArguments& InArgs, TSharedPtr<FModioModInfo>& InModInfo,
									  UModioSubsystem* InModioSubsystem)
{
	ModioSubsystem = InModioSubsystem;
	ModInfo = InModInfo;

	// clang-format off
	ChildSlot
	[
			SNew(SHorizontalBox) 
			+ SHorizontalBox::Slot()
			.Padding(8.f)
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.FillWidth(1)
			[
				SAssignNew(ImageBox, SBox)
				.MinDesiredWidth(68)
				.MinDesiredHeight(38)
				.MaxDesiredWidth(68)
				.MaxDesiredHeight(38)
				.MinAspectRatio(320.f/180.f)
				.MaxAspectRatio(320.f/180.f)
			] 
			+ SHorizontalBox::Slot()
			.Padding(4.f, 2.f)
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.FillWidth(4)
			[
				SNew(STextBlock)
				.Text(FText::FromString(ModInfo.Get()->ProfileName))
			] 
			+ SHorizontalBox::Slot()
			.Padding(4.f, 2.f)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			.FillWidth(1)
			[
				SNew(STextBlock)
				.Text(FText::FromString(ModInfo.Get()->ProfileDateAdded.ToFormattedString(TEXT("%b %d, %Y"))))
			]
			+ SHorizontalBox::Slot()
			.FillWidth(1)
			.Padding(4.f, 2.f)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(FText::FromString(GetStatusStringFromEnum(ModInfo.Get()->ModStatus)))
			]
			+ SHorizontalBox::Slot()
			.FillWidth(1)
			.Padding(4.f, 2.f)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(FText::FromString(GetVisibilityStringFromEnum(ModInfo.Get()->Visibility)))
			]
	];
	// clang-format on

	DrawThumbnailImage();
}

void SModioModBrowserEntry::DrawThumbnailImage()
{
	// clang-format off
	ImageBox->SetContent(
		SNew(SOverlay)
		+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SImage)
		]
		+ SOverlay::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(SThrobber)
		]
	);

	// clang-format on
	ModioSubsystem->GetModMediaAsync(
		ModInfo.Get()->ModId, EModioLogoSize::Thumb320,
		FOnGetMediaDelegateFast::CreateSP(this, &SModioModBrowserEntry::OnImageDownloaded));
}

void SModioModBrowserEntry::OnImageDownloaded(FModioErrorCode ErrorCode, TOptional<FModioImageWrapper> Image)
{
	if (ErrorCode)
	{
		return;
	}

	Image->LoadAsync(FOnLoadImageDelegateFast::CreateSP(this, &SModioModBrowserEntry::OnImageLoaded));
}

void SModioModBrowserEntry::OnImageLoaded(UTexture2DDynamic* LoadedImage)
{
	if (!LoadedImage)
	{
		return;
	}

	ThumbnailImageBrush =
		MakeShareable(new FSlateImageBrush(LoadedImage, FVector2D(LoadedImage->SizeX, LoadedImage->SizeY)));

	// clang-format off
	ImageBox->SetContent(
		SNew(SImage)
		.Image(ThumbnailImageBrush.Get())
	);
	// clang-format on
}

const FString SModioModBrowserEntry::GetStatusStringFromEnum(EModioModServerSideStatus StatusEnum) const
{
	FString StatusEnumAsString = UEnum::GetValueAsString(StatusEnum);
	StatusEnumAsString.RemoveFromStart(TEXT("EModioModServerSideStatus::"));
	return StatusEnumAsString;
}

const FString SModioModBrowserEntry::GetVisibilityStringFromEnum(EModioObjectVisibilityFlags VisibilityFlags) const
{
	switch (VisibilityFlags)
	{
		case EModioObjectVisibilityFlags::Hidden:
			return "Hidden";
		case EModioObjectVisibilityFlags::Public:
			return "Public";
		default:
			return "Invalid";
	}
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE