// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Views/CategoryBrowser/ModioModCarousel.h"
#include "Core/ModioModInfoUI.h"
#include "SWidgetCarouselWithNavigation.h"
#include "UI/BaseWidgets/ModioUserWidgetBase.h"
#include "WidgetCarouselStyle.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Layout/SBox.h"

void UModioCarousel::SynchronizeProperties()
{
	Super::SynchronizeProperties();
}

void UModioCarousel::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	MyCarouselWidget.Reset();
	EntryWidgets.Empty();
}

TSharedRef<SWidget> UModioCarousel::OnGenerateWidget(UModioModInfoUI* Item)
{
	if (EntryWidgetClass)
	{
		UUserWidget* NewWidget = CreateWidget<UUserWidget>(this, *EntryWidgetClass);
		NewWidget->SetClipping(EWidgetClipping::ClipToBoundsWithoutIntersecting);
		UModioUserWidgetBase* RealWidget = Cast<UModioUserWidgetBase>(NewWidget);
		if (RealWidget)
		{
			RealWidget->SetDataSource(Item);
		}
		EntryWidgets.Add(NewWidget);
		return NewWidget->TakeWidget();
	}
	else
	{
		return SNew(SBox);
	}
}

/*
FReply UModioModCarousel::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	TArray<FKey> MappedNavigateLeft = GetKeyForModioInputName(ModioInputNames::ModioInput_Left);
	if (MappedNavigateLeft.ContainsByPredicate([InKeyEvent](FKey Mapping) { return Mapping == InKeyEvent.GetKey(); }))
	{
		//NavigateLeft();
		return FReply::Handled();
	}
	TArray<FKey> MappedNavigateRight = GetKeyForModioInputName(ModioInputNames::ModioInput_Right);
	if (MappedNavigateRight.ContainsByPredicate([InKeyEvent](FKey Mapping) { return Mapping == InKeyEvent.GetKey(); }))
	{
		//NavigateRight();
		return FReply::Handled();
	}
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}
*/

TSharedRef<SWidget> UModioCarousel::RebuildWidget()
{
	if (IsDesignTime())
	{
		ModsToDisplay.Empty();
		ModsToDisplay.Add(NewObject<UModioModInfoUI>());
		ModsToDisplay.Add(NewObject<UModioModInfoUI>());
		ModsToDisplay.Add(NewObject<UModioModInfoUI>());
	}
	EntryWidgets.Empty();
	return SAssignNew(MyCarouselWidget, SWidgetCarouselWithNavigation<UModioModInfoUI*>)
		.NavigationBarStyle(FWidgetCarouselModuleStyle::Get(), "CarouselNavigationBar")
		.NavigationButtonStyle(FWidgetCarouselModuleStyle::Get(), "CarouselNavigationButton")
		.OnGenerateWidget(SWidgetCarouselWithNavigation<UModioModInfoUI*>::FOnGenerateWidget::CreateUObject(
			this, &UModioCarousel::OnGenerateWidget))
		.WidgetItemsSource(&ModsToDisplay);
}

void UModioCarousel::SetItems(TArray<UModioModInfoUI*> Items)
{
	EntryWidgets.Empty();
	ModsToDisplay = Items;
	RebuildWidget();
}
