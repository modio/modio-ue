#include "UI/Default/Gallery/ModioCommonModGalleryView.h"

#include "ModioSubsystem.h"
#include "ModioUI5.h"
#include "ModioUISubsystem.h"
#include "UI/Foundation/Components/Button/ModioCommonButtonBase.h"
#include "UI/Foundation/Components/Image/ModioCommonImage.h"
#include "Components/ListView.h"
#include "UI/Default/Gallery/ModioCommonModGalleryEntry.h"
#include "UI/Default/Gallery/ModioCommonModGalleryViewStyle.h"
#include "UI/Foundation/Components/Image/ModioCommonDynamicImage.h"
#include "UI/Foundation/Components/Image/ModioCommonDynamicImageStyle.h"
#include "UI/Foundation/Components/List/ModioCommonListView.h"
#include "UI/Settings/ModioCommonUISettings.h"
#include "UI/Settings/Params/ModioCommonModGalleryParams.h"

UModioCommonModGalleryView::UModioCommonModGalleryView()
{
	bAutoBindInputAction = false;
}

void UModioCommonModGalleryView::SetStyle(TSubclassOf<UModioCommonModGalleryViewStyle> InStyle)
{
	if (InStyle && InStyle != ModioStyle)
	{
		ModioStyle = InStyle;
		SynchronizeProperties();
	}
}

void UModioCommonModGalleryView::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (UModioCommonModGalleryViewStyle* StyleCDO = ModioStyle.GetDefaultObject())
	{
		if (SelectedGalleryImage)
		{
			if (TSubclassOf<UModioCommonDynamicImageStyle> ImageStyle = StyleCDO->GalleryImageStyle)
			{
				SelectedGalleryImage->SetStyle(ImageStyle);
			}
		}
	}
}

void UModioCommonModGalleryView::NativeOnInitialized() 
{
	Super::NativeOnInitialized();

	if (NextButton)
	{
		NextButton->OnClicked().AddWeakLambda(this, [this]() 
		{
			GoToNextImage(); 
		});
	}

	if (PreviousButton)
	{
		PreviousButton->OnClicked().AddWeakLambda(this, [this]() 
		{ 
			GoToPreviousImage(); 
		});
	}

	if (ImageNavButtons)
	{
		ImageNavButtons->OnItemClicked().AddWeakLambda(this, [this](UObject* Item) 
		{ 
			if (UModioCommonModGalleryItem* GalleryItem = Cast<UModioCommonModGalleryItem>(Item))
			{
				CurrentImageGalleryIndex = GalleryItem->ImageGalleryIndex;
				RefreshGallery();
			}
		});
	}
}

void UModioCommonModGalleryView::NativeOnSetDataSource()
{
	Super::NativeOnSetDataSource();

	if (!DataSource)
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to set up gallery for '%s': empty DataSource"), *GetName());
		return;
	}

	if (!DataSource->Implements<UModioModInfoUIDetails>())
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to set up gallery for '%s': DataSource '%s' does not implement UModioModInfoUIDetails"), *GetName(), *DataSource->GetName());
	}

	const FModioModInfo ModInfo = IModioModInfoUIDetails::Execute_GetFullModInfo(DataSource);
	
	UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
	if (!Subsystem)
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to set up gallery for '%s': unable to get ModioUISubsystem"), *GetName());
		return;
	}

	if (ImageNavButtons)
	{
		ImageNavButtons->ClearListItems();
	}
	else
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to set up gallery for '%s': ImageNavButtons is not bound"), *GetName());
	}

	const bool bAnyGalleryImages = ModInfo.NumGalleryImages > 0;
	CurrentImageGalleryIndex = bAnyGalleryImages ? 0 : -1;

	for (int32 GalleryImageIndex = 0; GalleryImageIndex < ModInfo.NumGalleryImages; GalleryImageIndex++)
	{
		AddGalleryImage(GalleryImageIndex);
	}

	// If there are no gallery images, add a default image
	if (!bAnyGalleryImages)
	{
		AddDefaultGalleryImage();
	}

	RefreshGallery();
}

void UModioCommonModGalleryView::BindInputActions()
{
	Super::BindInputActions();

	if (CurrentImageGalleryIndex <= 0)
	{
		SetPreviousButtonVisibility(false);
	}
	else
	{
		SetPreviousButtonVisibility(true);
	}

	if (CurrentImageGalleryIndex >= GetNumGalleryImages() - 1)
	{
		SetNextButtonVisibility(false);
	}
	else
	{
		SetNextButtonVisibility(true);
	}
}

void UModioCommonModGalleryView::UnbindInputActions()
{
	Super::UnbindInputActions();

	SetPreviousButtonVisibility(false);
	SetNextButtonVisibility(false);
}

void UModioCommonModGalleryView::RefreshGallery_Implementation()
{
	BindInputActions();

	if (!DataSource || !DataSource->Implements<UModioModInfoUIDetails>())
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to refresh gallery for '%s': empty DataSource or DataSource does not implement UModioModInfoUIDetails"), *GetName());
		return;
	}

	const FModioModID ModId = IModioModInfoUIDetails::Execute_GetModID(DataSource);

	if (SelectedGalleryImage && GetNumGalleryImages() > 0)
	{
		SelectedGalleryImage->LoadImageFromGallery(ModId, EModioGallerySize::Original, CurrentImageGalleryIndex);
		if (ImageNavButtons)
		{
			ImageNavButtons->SetSelectedIndex(CurrentImageGalleryIndex);
			ImageNavButtons->ScrollIndexIntoView(CurrentImageGalleryIndex);
		}
	}
	else if (SelectedGalleryImage)
	{
		SelectedGalleryImage->LoadImageFromLogo(ModId, EModioLogoSize::Thumb1280);
	}

	const ESlateVisibility ControlsVisibility = GetNumGalleryImages() > 1 ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed;
	if (ImageNavButtons) ImageNavButtons->SetVisibility(ControlsVisibility);
	if (PreviousButton) PreviousButton->SetVisibility(ControlsVisibility);
	if (NextButton) NextButton->SetVisibility(ControlsVisibility);
}

void UModioCommonModGalleryView::SetPreviousButtonVisibility_Implementation(bool bVisible)
{
	if (!PreviousButton)
	{
		return;
	}

	const UModioCommonUISettings* UISettings = GetDefault<UModioCommonUISettings>();
	if (!UISettings)
	{
		return;
	}

	if (bVisible && !UISettings->ModGalleryParams.PreviousImageInputAction.IsNull())
	{
		PreviousButton->SetTriggeringInputAction(UISettings->ModGalleryParams.PreviousImageInputAction);
		PreviousButton->SetIsInteractionEnabled(true);
	}
	else
	{
		PreviousButton->SetTriggeringInputAction(FDataTableRowHandle());
		PreviousButton->SetIsInteractionEnabled(false);
	}
}

void UModioCommonModGalleryView::SetNextButtonVisibility_Implementation(bool bVisible)
{
	if (!NextButton)
	{
		return;
	}

	const UModioCommonUISettings* UISettings = GetDefault<UModioCommonUISettings>();
	if (!UISettings)
	{
		return;
	}

	if (bVisible && !UISettings->ModGalleryParams.NextImageInputAction.IsNull())
	{
		NextButton->SetTriggeringInputAction(UISettings->ModGalleryParams.NextImageInputAction);
		NextButton->SetIsInteractionEnabled(true);
	}
	else
	{
		NextButton->SetTriggeringInputAction(FDataTableRowHandle());
		NextButton->SetIsInteractionEnabled(false);
	}
}

void UModioCommonModGalleryView::GoToNextImage_Implementation()
{
	CurrentImageGalleryIndex++;

	if (!FMath::IsWithinInclusive(CurrentImageGalleryIndex, 0, GetNumGalleryImages() - 1))
	{
		CurrentImageGalleryIndex = 0;
	}

	RefreshGallery();
}

void UModioCommonModGalleryView::GoToPreviousImage_Implementation()
{
	CurrentImageGalleryIndex--;

	if (!FMath::IsWithinInclusive(CurrentImageGalleryIndex, 0, GetNumGalleryImages() - 1))
	{
		CurrentImageGalleryIndex = GetNumGalleryImages() - 1;
	}

	RefreshGallery();
}

int32 UModioCommonModGalleryView::GetCurrentImageGalleryIndex_Implementation()
{
	return CurrentImageGalleryIndex;
}

int32 UModioCommonModGalleryView::GetNumGalleryImages_Implementation() 
{
	if (!DataSource || !DataSource->Implements<UModioModInfoUIDetails>())
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to get number of gallery images for '%s': empty DataSource or DataSource does not implement UModioModInfoUIDetails"), *GetName());
		return 0;
	}
	return IModioModInfoUIDetails::Execute_GetFullModInfo(DataSource).NumGalleryImages;
}

void UModioCommonModGalleryView::AddDefaultGalleryImage_Implementation()
{
	AddGalleryImage(-1);
}

void UModioCommonModGalleryView::AddGalleryImage_Implementation(int32 ImageGalleryIndex)
{
	if (!DataSource || !DataSource->Implements<UModioModInfoUIDetails>())
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to add gallery image for '%s': empty DataSource or DataSource does not implement UModioModInfoUIDetails"), *GetName());
		return;
	}

	if (!ImageNavButtons)
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to add gallery image for '%s': ImageNavButtons is not bound"), *GetName());
	}

	UModioCommonModGalleryItem* ImageGalleryItem = NewObject<UModioCommonModGalleryItem>();
	if (ImageGalleryItem)
	{
		ImageGalleryItem->ModInfo = IModioModInfoUIDetails::Execute_GetFullModInfo(DataSource);
		ImageGalleryItem->ImageGalleryIndex = ImageGalleryIndex;
		ImageNavButtons->AddItem(ImageGalleryItem);
	}
}
