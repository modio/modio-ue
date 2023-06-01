/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/Views/ModDetails/ModioModDetailsImageGallery.h"
#include "UI/Views/ModDetails/ModioModDetailsImage.h"
#include "Brushes/SlateImageBrush.h"
#include "Core/ModioModInfoUI.h"
#include "Engine/Engine.h"
#include "Engine/Texture2DDynamic.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "ModioUISubsystem.h"
#include "UI/BaseWidgets/ModioGridPanel.h"
#include "UI/Commands/ModioCommonUICommands.h"
#include "UI/Interfaces/IModioUIImageDisplay.h"
#include "UI/Styles/ModioUIStyleSet.h"

void UModioModDetailsImageGallery::OnGalleryImageLoad(class UTexture2DDynamic* LoadedImage, FModioModID ModID,
													  int32 Index)
{
	if (DataSource)
	{
		UModioModInfoUI* ModInfo = Cast<UModioModInfoUI>(DataSource);
		// Using the mod ID to ensure that if we somehow changed our data source while the request was in flight, we
		// only do anything if the mod ID is the same
		if (ModInfo && ModInfo->Underlying.ModId == ModID)
		{
			ImageInfo[Index].LoadedImage = LoadedImage;
		}
		if (Index == CurrentImageIndex)
		{
			if (ImageGallery)
			{
				ImageGallery->RefreshCurrentImage();
			}
		}
	}
}

void UModioModDetailsImageGallery::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (ImageGallery)
	{
		ImageGallery->GetImageDisplayDelegate().BindDynamic(this, &UModioModDetailsImageGallery::ApplyImageToWidget);
		ImageGallery->GetItemCountDelegate().BindDynamic(this, &UModioModDetailsImageGallery::GetImageCount);
	}
	if (NavButtons)
	{
		NavButtons->OnItemClicked().AddUObject(this, &UModioModDetailsImageGallery::OnNavButtonClicked);
	}
	IModioUIMediaDownloadCompletedReceiver::Register<UModioModDetailsImageGallery>(
		EModioUIMediaDownloadEventType::ModGalleryImages | EModioUIMediaDownloadEventType::ModLogo);
}

void UModioModDetailsImageGallery::NativeOnSetDataSource()
{
	Super::NativeOnSetDataSource();
	CurrentImageIndex = 0;
	if (DataSource)
	{
		UModioModInfoUI* ModInfo = Cast<UModioModInfoUI>(DataSource);
		if (ModInfo)
		{
			if (ModInfo->Underlying.NumGalleryImages > 0)
			{
				ImageInfo.Empty(ModInfo->Underlying.NumGalleryImages);
				ImageInfo.SetNumZeroed(ModInfo->Underlying.NumGalleryImages);
			}
			// if there are no gallery images, ImageInfo will only have one member, modlogo
			else
			{
				ImageInfo.Empty(1);
				ImageInfo.SetNumZeroed(1);
			}

			if (ModInfo->Underlying.NumGalleryImages <= 1)
			{
				NavButtonGridPanel->SetVisibility(ESlateVisibility::Collapsed);
			}
			else
			{
				NavButtonGridPanel->SetVisibility(ESlateVisibility::Visible);
			}

			

			TArray<TSharedPtr<int64>> ImageIndexes;
			if (ModInfo->Underlying.NumGalleryImages == 0)
			{
				ImageIndexes.Add(MakeShared<int64>(0));
			}
			
			UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
			
			for (int32 ImageIndex = 0; ImageIndex < ModInfo->Underlying.NumGalleryImages; ImageIndex++)
			{
				if (!IsDesignTime())
				{
					if (Subsystem)
					{
						Subsystem->RequestGalleryImageDownloadForModID(ModInfo->Underlying.ModId, ImageIndex);
					}
				}
				ImageIndexes.Add(MakeShared<int64>(ImageIndex));

			}

			if (NavButtons)
			{
				NavButtons->SetListItems(ImageIndexes);

				NavButtons->ClearSelection();
				NavButtons->SetSelectedItem(ImageIndexes[0], ESelectInfo::Direct);
			}

			if (ImageIndexes.Num())
			{
				ImageGallery->ResetGallery();
			}

			// Ensure this is called last, as it invokes an async callback which, if called earlier, *may* be invoked
			// before the gallery is reset and cause a crash
			// This will be called only if there are no other gallery images to show
			if (Subsystem && ModInfo->Underlying.NumGalleryImages == 0)
			{
				Subsystem->RequestLogoDownloadForModID(ModInfo->Underlying.ModId, EModioLogoSize::Thumb1280);
			}
		}
	}
}

void UModioModDetailsImageGallery::NativeOnModLogoDownloadCompleted(FModioModID ModID, FModioErrorCode ec,
																	TOptional<FModioImageWrapper> Image)
{
	IModioUIMediaDownloadCompletedReceiver::NativeOnModLogoDownloadCompleted(ModID, ec, Image);
	UModioModInfoUI* ModInfo = Cast<UModioModInfoUI>(DataSource);

	if (DataSource && ModInfo && ModInfo->Underlying.NumGalleryImages == 0)
	{
		if (ModInfo->Underlying.ModId == ModID)
		{
			ImageInfo[0].Status = ec;
			ImageInfo[0].DownloadedImageReference = Image;
			if (!ec && Image)
			{
				Image->LoadAsync(FOnLoadImageDelegateFast::CreateUObject(
					this, &UModioModDetailsImageGallery::OnGalleryImageLoad, ModID, 0));
			}
		}
	}
}

void UModioModDetailsImageGallery::NativeOnModGalleryImageDownloadCompleted(FModioModID ModID, FModioErrorCode ec,
																			int32 ImageIndex,
																			TOptional<FModioImageWrapper> Image)
{
	IModioUIMediaDownloadCompletedReceiver::NativeOnModGalleryImageDownloadCompleted(ModID, ec, ImageIndex, Image);
	if (DataSource)
	{
		UModioModInfoUI* ModInfo = Cast<UModioModInfoUI>(DataSource);

		 if (ModInfo && ModInfo->Underlying.ModId == ModID)
		{
			ImageInfo[ImageIndex].Status = ec;
			ImageInfo[ImageIndex].DownloadedImageReference = Image;

			if (!ec && Image)
			{
				Image->LoadAsync(FOnLoadImageDelegateFast::CreateUObject(
					this, &UModioModDetailsImageGallery::OnGalleryImageLoad, ModID, ImageIndex));
			}
		}
	}
}

FReply UModioModDetailsImageGallery::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	bIsFocused = true;

	const FModioDynamicImageStyle* ResolvedStyle = Style.FindStyle<FModioDynamicImageStyle>();
	if (ResolvedStyle)
	{
		FSlateApplication::Get().PlaySound(ResolvedStyle->HoveredSound);
	}

	ActiveBackground->SetVisibility(ESlateVisibility::Visible);
	SetKeyboardFocus();

	return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
}

void UModioModDetailsImageGallery::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{
	bIsFocused = false;
	ActiveBackground->SetVisibility(ESlateVisibility::Collapsed);
}

void UModioModDetailsImageGallery::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	bIsFocused = true;

	const FModioDynamicImageStyle* ResolvedStyle = Style.FindStyle<FModioDynamicImageStyle>();
	if (ResolvedStyle)
	{
		FSlateApplication::Get().PlaySound(ResolvedStyle->HoveredSound);
	}

	ActiveBackground->SetVisibility(ESlateVisibility::Visible);
	SetKeyboardFocus();

	return Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

void UModioModDetailsImageGallery::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	bIsFocused = false;
	ActiveBackground->SetVisibility(ESlateVisibility::Collapsed);
}

void UModioModDetailsImageGallery::PrevImage()
{
	ShowImageInternal(CurrentImageIndex - 1);
	UModioModInfoUI* ModInfo = Cast<UModioModInfoUI>(DataSource);

	if (NavButtons)
	{
		if (IsValid(ModInfo) && ModInfo->Underlying.NumGalleryImages > 1)
		{
			NavButtons->ClearSelection();
			NavButtons->SelectSingleItem(CurrentImageIndex);
		}
	}
}

void UModioModDetailsImageGallery::NextImage()
{
	ShowImageInternal(CurrentImageIndex + 1);
	UModioModInfoUI* ModInfo = Cast<UModioModInfoUI>(DataSource);

	if (NavButtons)
	{
		if (IsValid(ModInfo) && ModInfo->Underlying.NumGalleryImages > 1)
		{
			NavButtons->ClearSelection();
			NavButtons->SelectSingleItem(CurrentImageIndex);
		}
	}
}

void UModioModDetailsImageGallery::ApplyImageToWidget(UWidget* Widget, int32 ImageIndex)
{
	if ((ImageIndex + 1) > ImageInfo.Num())
		return;

	UTexture2DDynamic* TargetImage = ImageInfo[ImageIndex].LoadedImage;
	{
		if (TargetImage)
		{
			FVector2D ImageDimensions(TargetImage->SizeX, TargetImage->SizeY);
			IModioUIImageDisplay::Execute_DisplayImage(Widget, FSlateImageBrush(TargetImage, Widget->GetDesiredSize()));
		}
		else
		{
			// when starting, the code runs here before the async operation is ready,
			// so we want loading brush instead of error, where it would otherwise go
			if (ImageInfo[ImageIndex].DownloadedImageReference.IsSet() && ImageInfo[ImageIndex].DownloadedImageReference.GetValue().GetState() == EModioImageState::LoadingIntoMemory)
			{
				UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
				if (Subsystem)
				{
					UModioUIStyleSet* DefaultStyleSet = Subsystem->GetDefaultStyleSet();
					if (DefaultStyleSet)
					{
						TSharedPtr<FSlateBrush> LoadingBrush =
							DefaultStyleSet->GetMaterialBrush(FName("Loading"), {}, FVector2D(256, 256));
						IModioUIImageDisplay::Execute_DisplayImage(Widget, *LoadingBrush);
					}
				}
			}
			else if (ImageInfo[ImageIndex].Status)
			{
				UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
				if (Subsystem)
				{
					UModioUIStyleSet* DefaultStyleSet = Subsystem->GetDefaultStyleSet();
					if (DefaultStyleSet)
					{
						TSharedPtr<FSlateBrush> ErrorBrush =
							DefaultStyleSet->GetMaterialBrush(FName("ImageError"), {}, FVector2D(256, 256));
						IModioUIImageDisplay::Execute_DisplayImage(Widget, *ErrorBrush);
					}
				}
			}
			else
			{
				UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
				if (Subsystem)
				{
					UModioUIStyleSet* DefaultStyleSet = Subsystem->GetDefaultStyleSet();
					if (DefaultStyleSet)
					{
						TSharedPtr<FSlateBrush> LoadingBrush = 
							DefaultStyleSet->GetMaterialBrush(FName("Loading"), {}, FVector2D(256, 256));
						IModioUIImageDisplay::Execute_DisplayImage(Widget, *LoadingBrush);
					}
				}
			}
		}
	}
}

int32 UModioModDetailsImageGallery::GetImageCount()
{
	return ImageInfo.Num();
}

void UModioModDetailsImageGallery::OnNavButtonClicked(TSharedPtr<int64> Item)
{
	if (!Item || *Item == CurrentImageIndex)
	{
		return;
	}

	UModioModInfoUI* ModInfo = Cast<UModioModInfoUI>(DataSource);
	if (!ModInfo)
	{
		// log
		return;
	}

	ShowImageInternal(*Item);
}

void UModioModDetailsImageGallery::ShowImageInternal(int32 RawIndex)
{
	if (UModioModInfoUI* ModInfo = Cast<UModioModInfoUI>(DataSource))
	{
		int32 NewIndex = 0;

		(ModInfo->Underlying.NumGalleryImages > 0) ? NewIndex = RawIndex % (ModInfo->Underlying.NumGalleryImages) : NewIndex = 0;

		if (RawIndex < 0 && ModInfo->Underlying.NumGalleryImages > 0)
		{
			NewIndex = ModInfo->Underlying.NumGalleryImages - 1;
		}

		// int32 NewIndex = FMath::Clamp<int32>(RawIndex, 0, ModInfo->Underlying.NumGalleryImages);
		if (ImageGallery)
		{
			// if the new index is higher, we want our new image to move in *from* the right
			ImageGallery->DisplayGalleryImageAtIndex(
				NewIndex > CurrentImageIndex ? EModioUIDirection::MoveLeft : EModioUIDirection::MoveRight, NewIndex);
		}
		CurrentImageIndex = NewIndex;
	}
}

void UModioModDetailsImageGallery::ChangeImage(const FKey& InputKey)
{
	if (InputKey == FModioInputKeys::Next)
	{
		NextImage();
	}
	else if (InputKey == FModioInputKeys::Previous)
	{
		PrevImage();
	}
}

void UModioModDetailsImageGalleryNavButtonBase::NativePreConstruct()
{
	UModioUserWidgetBase::NativePreConstruct();
	if (Image)
	{
		if (!SpecifiedMaterial.IsNull())
		{
			UMaterialInstanceDynamic* WidgetMaterial = UMaterialInstanceDynamic::Create(SpecifiedMaterial.Get(), this);
			WidgetMaterial->SetScalarParameterValue(FName("UseRenderTarget"), 0);
			WidgetMaterial->SetVectorParameterValue(FName("Color"), UnselectedColor.GetSpecifiedColor());
			Image->SetBrushFromMaterial(WidgetMaterial);
		}
	}
}

void UModioModDetailsImageGalleryNavButtonBase::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	UMaterialInstanceDynamic* WidgetMaterial = Image->GetDynamicMaterial();

	WidgetMaterial->SetVectorParameterValue(FName("Color"), UnselectedColor.GetSpecifiedColor());
}

void UModioModDetailsImageGalleryNavButtonBase::NativeOnItemSelectionChanged(bool bIsSelected)
{
	UMaterialInstanceDynamic* WidgetMaterial = Image->GetDynamicMaterial();
	if (bIsSelected)
	{
		WidgetMaterial->SetVectorParameterValue(FName("Color"), SelectedColor.GetSpecifiedColor());
	}
	else
	{
		WidgetMaterial->SetVectorParameterValue(FName("Color"), UnselectedColor.GetSpecifiedColor());
	}
}
