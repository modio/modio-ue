// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/CommonComponents/ModioImageGalleryBase.h"
#include "Core/ModioModInfoUI.h"
#include "Types/ModioModInfo.h"
#include "Widgets/Layout/SFxWidget.h"

/*
void UModioImageGallery::NativeOnInitialized()
{
	IModioUIMediaDownloadCompletedReceiver::Register<UModioImageGallery>(
		EModioUIMediaDownloadEventType::ModGalleryImages);
}*/

TSharedRef<SWidget> UModioImageGalleryBase::RebuildWidget()
{
	ForegroundContent = GenerateContentWidget();
	BackgroundContent = GenerateContentWidget();
	// clang-format off
	MyGalleryRootWidget =
		SNew(SOverlay) 
		+SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SAssignNew(BackgroundImageWrapper, SFxWidget)
			.Content()
			[
				BackgroundContent ? BackgroundContent->TakeWidget()
									: SNullWidget::NullWidget
			]
		] 
		+SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SAssignNew(ForegroundImageWrapper, SFxWidget)
			.Content()
			[
				ForegroundContent ? ForegroundContent->TakeWidget() 
									: SNullWidget::NullWidget
			]
		];
	// clang-format on
	return MyGalleryRootWidget.ToSharedRef();
}

void UModioImageGalleryBase::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	MyGalleryRootWidget.Reset();
	BackgroundImageWrapper.Reset();
	ForegroundImageWrapper.Reset();
}

/*
void UModioImageGallery::NativeOnSetDataSource()
{
	Super::NativeOnSetDataSource();
	ActualModInfo = Cast<UModioModInfoUI>(DataSource);
}
*/

void UModioImageGalleryBase::SynchronizeProperties()
{
	Super::SynchronizeProperties();
}

UWidget* UModioImageGalleryBase::GenerateContentWidget()
{
	if (OnConstructWidget.IsBound())
	{
		UWidget* ContentWidget = OnConstructWidget.Execute();
		if (ContentWidget)
		{
			return ContentWidget;
		}
	}
	else if (ImageWidgetClass != nullptr && !ImageWidgetClass->HasAnyClassFlags(CLASS_Abstract))
	{
		if (UUserWidget* ImageWidget = CreateWidget(this, ImageWidgetClass))
		{
			return ImageWidget;
		}
	}
	return nullptr;
}

void UModioImageGalleryBase::DisplayGalleryImageAtIndex(EModioUIDirection Direction, int32 Index)
{
	if (Index == CurrentImageIndex)
	{
		// noop
		return;
	}

	UWidget* OldBackgroundContent = BackgroundContent;
	BackgroundContent = ForegroundContent;
	ForegroundContent = OldBackgroundContent;

	if (Index != INDEX_NONE)
	{
		CurrentImageIndex = Index;
	}
	else
	{
		if (OnGetItemCount.IsBound())
		{
			// Moving left means we are increasing
			if (Direction == EModioUIDirection::MoveLeft)
			{
				CurrentImageIndex++;
			}
			else
			{
				CurrentImageIndex--;
			}

			if (CurrentImageIndex >= OnGetItemCount.Execute())
			{
				CurrentImageIndex = 0;
			}
			if (CurrentImageIndex < 0)
			{
				CurrentImageIndex = OnGetItemCount.Execute() - 1;
			}
		}
		else
		{
			// If the user is asking us to move forward or backward and we don't know how many items there are, don't do
			// anything because we can't update the CurrentImageIndex;
			return;
		}
	}

	if (OnDisplayImage.IsBound())
	{
		OnDisplayImage.Execute(ForegroundContent, CurrentImageIndex);
	}
	MyGalleryRootWidget->ClearChildren();

	MyGalleryRootWidget->AddSlot()[BackgroundContent->TakeWidget()];
	MyGalleryRootWidget->AddSlot()[ForegroundContent->TakeWidget()];

	// begin transition (direction)
}

void UModioImageGalleryBase::ResetGallery()
{
	CurrentImageIndex = 0;
	if (OnDisplayImage.IsBound())
	{
		if (ForegroundContent)
		{
			OnDisplayImage.Execute(ForegroundContent, CurrentImageIndex);	
		}
	}
}

void UModioImageGalleryBase::RefreshCurrentImage()
{
	if (OnDisplayImage.IsBound())
	{
		if (ForegroundContent)
		{
			OnDisplayImage.Execute(ForegroundContent, CurrentImageIndex);	
		}
	}
}

/*

void UModioImageGallery::NativeOnModGalleryImageDownloadCompleted(FModioModID ModID, FModioErrorCode ec,
																  int32 ImageIndex, TOptional<FModioImageWrapper> Image)
{
	if (ActualModInfo && ActualModInfo->Underlying.ModId == ModID)
	{
		ImageDownloadStatus[ImageIndex] = ec;
		if (!ec)
		{
			GalleryImages[ImageIndex] = Image;
		}
	}
}

void UModioImageGallery::DisplayGalleryImageAtIndex(int32 Index)
{
	if (ActualModInfo)
	{
		int32 SafeIndex = FMath::Clamp(Index, 0, ActualModInfo->Underlying.NumGalleryImages);
		if (ImageDownloadStatus.Contains(Index))
		{
			if (ImageDownloadStatus[Index])
			{
				// Display error image;
			}
			else
			{
				// Display actual image
			}
		}
		else
		{}
	}
}
*/
