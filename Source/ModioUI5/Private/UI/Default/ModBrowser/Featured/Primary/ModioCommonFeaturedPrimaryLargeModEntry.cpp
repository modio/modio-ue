/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Default/ModBrowser/Featured/Primary/ModioCommonFeaturedPrimaryLargeModEntry.h"

#include "ModioUISubsystem.h"
#include "Core/ModioModInfoUI.h"
#include "Types/ModioModInfo.h"
#include "UI/Foundation/Components/Button/ModioCommonButtonBase.h"
#include "UI/Foundation/Base/Image/ModioCommonImageBase.h"
#include "UI/Foundation/Base/ModBrowser/ModioCommonModBrowserBase.h"
#include "UI/Foundation/Components/Text/RichTextBlock/ModioCommonRichTextBlock.h"
#include "UI/Foundation/Components/Text/RichTextBlock/ModioCommonRichTextStyle.h"
#include "UI/Foundation/Components/Text/TextBlock/ModioCommonTextBlock.h"
#include "UI/Foundation/Components/Text/TextBlock/ModioCommonTextBlockStyle.h"
#include "UI/Interfaces/IModioModBrowser.h"
#include "UI/Settings/Params/ModioCommonModEntryParams.h"

UModioCommonFeaturedPrimaryLargeModEntry::UModioCommonFeaturedPrimaryLargeModEntry()
{
	bIsFocusable = true;
}

void UModioCommonFeaturedPrimaryLargeModEntry::NativeOnItemSelectionChanged(bool bIsSelected)
{
	bSelected = bIsSelected;
	Super::NativeOnItemSelectionChanged(bIsSelected);
}

void UModioCommonFeaturedPrimaryLargeModEntry::NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent)
{
	// We should ignore unbinding input action on removal from focus path since it's warranted by design
	Super::Super::Super::NativeOnRemovedFromFocusPath(InFocusEvent);
}

bool UModioCommonFeaturedPrimaryLargeModEntry::IsModListItemValid_Implementation() const
{
	return true;
}

bool UModioCommonFeaturedPrimaryLargeModEntry::IsModListItemSelected_Implementation() const
{
	return bSelected;
}

void UModioCommonFeaturedPrimaryLargeModEntry::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	OnLargeModEntryMouseChanged.Broadcast(true);
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

void UModioCommonFeaturedPrimaryLargeModEntry::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	OnLargeModEntryMouseChanged.Broadcast(false);
}

void UModioCommonFeaturedPrimaryLargeModEntry::NativePreConstruct()
{
	Super::NativePreConstruct();
#if WITH_EDITOR
	if (IsDesignTime())
	{
		UModioModInfoUI* ModInfo = NewObject<UModioModInfoUI>();
		if (ModInfo)
		{
			ModInfo->Underlying = PreviewModInfo;
			SetDataSource(ModInfo);
		}
	}
#endif
}

UWidget* UModioCommonFeaturedPrimaryLargeModEntry::NativeGetDesiredFocusTarget() const
{
	if (UWidget* WidgetToFocus = BP_GetDesiredFocusTarget())
	{
		return WidgetToFocus;
	}
	return const_cast<UModioCommonFeaturedPrimaryLargeModEntry*>(this);
}

void UModioCommonFeaturedPrimaryLargeModEntry::NativeConstruct()
{
	Super::NativeConstruct();
	RefreshForegroundLayerVisibility();
	UnbindInputActions();
	SynchronizeProperties();
}

void UModioCommonFeaturedPrimaryLargeModEntry::NativeOnSetDataSource()
{
	Super::NativeOnSetDataSource();

	const UModioCommonModEntryParamsSettings* Settings = GetDefault<UModioCommonModEntryParamsSettings>();
	if (!Settings)
	{
		return;
	}

	RefreshForegroundLayerVisibility();
}

void UModioCommonFeaturedPrimaryLargeModEntry::RefreshForegroundLayerVisibility_Implementation()
{
	if (ForegroundContainer)
	{
		ForegroundContainer->SetVisibility(DataSource ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}