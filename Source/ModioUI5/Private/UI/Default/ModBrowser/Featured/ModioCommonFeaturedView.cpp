/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Default/ModBrowser/Featured/ModioCommonFeaturedView.h"

#include "UI/Default/ModBrowser/Featured/ModioCommonFeaturedViewStyle.h"
#include "UI/Default/ModBrowser/Featured/Additional/ModioCommonFeaturedAdditionalView.h"
#include "UI/Default/ModBrowser/Featured/Primary/ModioCommonFeaturedPrimaryView.h"

void UModioCommonFeaturedView::SetStyle(TSubclassOf<UModioCommonFeaturedViewStyle> InStyle)
{
	ModioStyle = InStyle;
	SynchronizeProperties();
}

void UModioCommonFeaturedView::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (const UModioCommonFeaturedViewStyle* StyleCDO = Cast<UModioCommonFeaturedViewStyle>(ModioStyle.GetDefaultObject()))
	{
		if (PrimaryView)
		{
			PrimaryView->SetStyle(StyleCDO->FeaturedPrimaryViewStyle);
		}

		if (AdditionalView)
		{
			AdditionalView->SetStyle(StyleCDO->FeaturedAdditionalViewStyle);
		}
	}
}

void UModioCommonFeaturedView::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (PrimaryView && AdditionalView)
	{
		if (PrimaryView->IsAnyModSelected() && !PrimaryView->HasFocusedDescendants() && AdditionalView->HasFocusedDescendants())
		{
			PrimaryView->ClearModSelection();
		}
		else if (AdditionalView->IsAnyModSelected() && !AdditionalView->HasFocusedDescendants() && PrimaryView->HasFocusedDescendants())
		{
			AdditionalView->ClearModSelection();
		}
	}
}

UWidget* UModioCommonFeaturedView::NativeGetDesiredFocusTarget() const
{
	if (UWidget* WidgetToFocus = BP_GetDesiredFocusTarget())
	{
		return WidgetToFocus;
	}
	if (PrimaryView && (PrimaryView->HasAnyUserFocus() || PrimaryView->HasFocusedDescendants()))
	{
		return PrimaryView->GetDesiredFocusTarget();
	}
	if (AdditionalView && (AdditionalView->HasAnyUserFocus() || AdditionalView->HasFocusedDescendants()))
	{
		return AdditionalView->GetDesiredFocusTarget();
	}
	if (LastFocusedView)
	{
		return LastFocusedView->GetDesiredFocusTarget();
	}
	if (PrimaryView)
	{
		return PrimaryView->GetDesiredFocusTarget();
	}
	return nullptr;
}

void UModioCommonFeaturedView::NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent)
{
	if (PrimaryView && (PrimaryView->HasAnyUserFocus() || PrimaryView->HasFocusedDescendants()))
	{
		LastFocusedView = PrimaryView;
	}
	if (AdditionalView && (AdditionalView->HasAnyUserFocus() || AdditionalView->HasFocusedDescendants()))
	{
		LastFocusedView = AdditionalView;
	}
	Super::NativeOnRemovedFromFocusPath(InFocusEvent);
}
