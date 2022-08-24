// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/CommonComponents/ModioWidgetCarousel.h"
#include "Blueprint/IUserObjectListEntry.h"

TSharedRef<SWidget> UModioWidgetCarousel::OnGenerateWidget(UObject* Item)
{
	if (EntryWidgetClass)
	{
		UModioUserWidgetBase* NewWidget = CreateWidget<UModioUserWidgetBase>(this, *EntryWidgetClass);
		if (!NewWidget)
		{
			return SNew(SBox);
		}
		
		// Do we need to hold a reference to the outer widget?
		TSharedRef<SWidget> SlateWidget = NewWidget->TakeWidget();
		//Doing this after TakeWidget because we should be definitely registered for events at that point
		NewWidget->SetClipping(EWidgetClipping::ClipToBoundsWithoutIntersecting);
		NewWidget->SetDataSource(Item);
		return SlateWidget;
	}
	else
	{
		return SNew(SBox);
	}
}

TSharedRef<SWidget> UModioWidgetCarousel::RebuildWidget()
{
	if (IsDesignTime() || ItemsSource.Num() == 0)
	{
		ItemsSource.Empty();
		ItemsSource.Add(nullptr);
		ItemsSource.Add(nullptr);
		ItemsSource.Add(nullptr);
	}
	LeftCarouselWidget = NewObject<UModioWidgetCarouselEntryWrapper>();
	CenterCarouselWidget = NewObject<UModioWidgetCarouselEntryWrapper>();
	RightCarouselWidget = NewObject<UModioWidgetCarouselEntryWrapper>();
	IncomingWidget = NewObject<UModioWidgetCarouselEntryWrapper>();

	return SAssignNew(MyCarousel, SModioWidgetCarouselBase<UObject*>)
		.OnGenerateWidget(SModioWidgetCarouselBase<UObject*>::FOnGenerateWidget::CreateUObject(
			this, &UModioWidgetCarousel::OnGenerateWidget))
		.ClippingBehaviour(EntryWidgetClipping)
		.WidgetItemsSource(ItemsSource)
		.LeftCarouselWidget(StaticCastSharedRef<SModioWidgetCarouselEntry>(LeftCarouselWidget->TakeWidget()))
		.CenterCarouselWidget(StaticCastSharedRef<SModioWidgetCarouselEntry>(CenterCarouselWidget->TakeWidget()))
		.RightCarouselWidget(StaticCastSharedRef<SModioWidgetCarouselEntry>(RightCarouselWidget->TakeWidget()))
		.IncomingWidget(StaticCastSharedRef<SModioWidgetCarouselEntry>(IncomingWidget->TakeWidget()))
		.OnLeftWidgetClicked(SModioWidgetCarouselBase<UObject*>::FOnCarouselEntryClicked::CreateUObject(
			this, &UModioWidgetCarousel::LeftWidgetClickedHandler))
		.OnCenterWidgetClicked(SModioWidgetCarouselBase<UObject*>::FOnCarouselEntryClicked::CreateUObject(
			this, &UModioWidgetCarousel::CenterWidgetClickedHandler))
		.OnRightWidgetClicked(SModioWidgetCarouselBase<UObject*>::FOnCarouselEntryClicked::CreateUObject(
			this, &UModioWidgetCarousel::RightWidgetClickedHandler))
		.RelativeWidgetSpacing(RelativeWidgetSpacing);
}

void UModioWidgetCarousel::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	MyCarousel->SetWidgetSpacing(RelativeWidgetSpacing);
	MyCarousel->SetItemSource(ItemsSource);
	switch (EntryWidgetSizeOverride)
	{
		case EModioWidgetCarouselSizeOverrideType::AbsoluteDimensions:
		{
			FOptionalSize WidthOverride;
			FOptionalSize HeightOverride;
			if (bOverrideAbsoluteHeight)
			{
				HeightOverride = FOptionalSize(EntryWidgetHeightOverride);
			}
			if (bOverrideAbsoluteWidth)
			{
				WidthOverride = FOptionalSize(EntryWidgetWidthOverride);
			}
			MyCarousel->SetWidgetSizeOverride(WidthOverride, HeightOverride,
											  EModioWidgetCarouselSizeOverrideType::AbsoluteDimensions);
		}
		break;
		case EModioWidgetCarouselSizeOverrideType::RelativeLayoutScale:
		{
			FOptionalSize WidthOverride;
			FOptionalSize HeightOverride;
			if (bOverrideRelativeLayoutHeight)
			{
				HeightOverride = FOptionalSize(EntryWidgetRelativeHeightOverride);
			}
			if (bOverrideRelativeLayoutWidth)
			{
				WidthOverride = FOptionalSize(EntryWidgetRelativeWidthOverride);
			}
			MyCarousel->SetWidgetSizeOverride(WidthOverride, HeightOverride,
											  EModioWidgetCarouselSizeOverrideType::RelativeLayoutScale);
		}
		break;
		default:
			MyCarousel->SetWidgetSizeOverride(FOptionalSize {}, FOptionalSize {},
											  EModioWidgetCarouselSizeOverrideType::NoOverride);
	}
}

void UModioWidgetCarousel::ReleaseSlateResources(bool bReleaseChildren)
{
	MyCarousel.Reset();
	Super::ReleaseSlateResources(bReleaseChildren);
}

void UModioWidgetCarousel::SetItems(TArray<UObject*> Items)
{
	ItemsSource = Items;
	if (MyCarousel)
	{
		MyCarousel->SetItemSource(ItemsSource);
	}
}

TSharedRef<SWidget> UModioWidgetCarouselEntryWrapper::RebuildWidget()
{
	return SNew(SModioWidgetCarouselEntry);
}

void UModioWidgetCarouselEntryWrapper::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
}
