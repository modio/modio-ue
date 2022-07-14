// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Views/ModDetails/ModioModDetailsImageGallery.h"
#include "Brushes/SlateImageBrush.h"
#include "Core/ModioModInfoUI.h"
#include "Engine/Engine.h"
#include "Engine/Texture2DDynamic.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "ModioUISubsystem.h"
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
			ImageInfo.Empty(ModInfo->Underlying.NumGalleryImages + 1);
			ImageInfo.SetNumZeroed(ModInfo->Underlying.NumGalleryImages + 1);

			TArray<TSharedPtr<int64>> ImageIndexes;
			ImageIndexes.Add(MakeShared<int64>(0));
			UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
			if (Subsystem)
			{
				Subsystem->RequestLogoDownloadForModID(ModInfo->Underlying.ModId, EModioLogoSize::Thumb1280);
			}
			for (int32 ImageIndex = 0; ImageIndex < ModInfo->Underlying.NumGalleryImages; ImageIndex++)
			{
				if (!IsDesignTime())
				{
					if (Subsystem)
					{
						Subsystem->RequestGalleryImageDownloadForModID(ModInfo->Underlying.ModId, ImageIndex);
					}
				}
				ImageIndexes.Add(MakeShared<int64>(ImageIndex + 1));
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
		}
	}
}

void UModioModDetailsImageGallery::NativeOnModLogoDownloadCompleted(FModioModID ModID, FModioErrorCode ec,
																	TOptional<FModioImageWrapper> Image)
{
	IModioUIMediaDownloadCompletedReceiver::NativeOnModLogoDownloadCompleted(ModID, ec, Image);
	if (DataSource)
	{
		UModioModInfoUI* ModInfo = Cast<UModioModInfoUI>(DataSource);
		if (ModInfo && ModInfo->Underlying.ModId == ModID)
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
			ImageInfo[ImageIndex + 1].Status = ec;
			ImageInfo[ImageIndex + 1].DownloadedImageReference = Image;
			if (!ec && Image)
			{
				Image->LoadAsync(FOnLoadImageDelegateFast::CreateUObject(
					this, &UModioModDetailsImageGallery::OnGalleryImageLoad, ModID, ImageIndex + 1));
			}
		}
	}
}

void UModioModDetailsImageGallery::BuildCommandList(TSharedRef<FUICommandList> CommandList)
{
	CommandList->MapAction(FModioCommonUICommands::Get().NextPage,
						   FUIAction(FExecuteAction::CreateUObject(this, &UModioModDetailsImageGallery::NextImage)));
	CommandList->MapAction(FModioCommonUICommands::Get().PreviousPage,
						   FUIAction(FExecuteAction::CreateUObject(this, &UModioModDetailsImageGallery::PrevImage)));
}

void UModioModDetailsImageGallery::PrevImage()
{
	ShowImageInternal(CurrentImageIndex - 1);
	if (NavButtons)
	{
		NavButtons->ClearSelection();
		NavButtons->SelectSingleItem(CurrentImageIndex);
	}
}

void UModioModDetailsImageGallery::NextImage()
{
	ShowImageInternal(CurrentImageIndex + 1);
	if (NavButtons)
	{
		NavButtons->ClearSelection();
		NavButtons->SelectSingleItem(CurrentImageIndex);
	}
}

void UModioModDetailsImageGallery::ApplyImageToWidget(UWidget* Widget, int32 ImageIndex)
{
	UTexture2DDynamic* TargetImage = ImageInfo[ImageIndex].LoadedImage;
	{
		if (TargetImage)
		{
			FVector2D ImageDimensions(TargetImage->SizeX, TargetImage->SizeY);
			IModioUIImageDisplay::Execute_DisplayImage(Widget, FSlateImageBrush(TargetImage, Widget->GetDesiredSize()));
		}
		else
		{
			if (ImageInfo[ImageIndex].Status)
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
		int32 NewIndex = RawIndex % (ModInfo->Underlying.NumGalleryImages + 1);
		if (RawIndex < 0)
		{
			NewIndex = ModInfo->Underlying.NumGalleryImages;
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
