/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Foundation/Components/List/ModioCommonTileView.h"

#include "CommonButtonBase.h"
#include "UI/Foundation/Components/List/ModioCommonListViewStyle.h"
#include "UI/Foundation/Components/List/SModioCommonButtonTableRow.h"

void UModioCommonTileView::SetStyle(TSubclassOf<UModioCommonListViewStyle> InStyle)
{
	if (InStyle && InStyle != ModioStyle)
	{
		ModioStyle = InStyle;
		SynchronizeProperties();
	}
}

void UModioCommonTileView::SynchronizeProperties()
{
	if (UModioCommonListViewStyle* StyleCDO = ModioStyle.GetDefaultObject())
	{
		WidgetStyle = StyleCDO->WidgetStyle;
		ScrollBarStyle = StyleCDO->ScrollBarStyle;
		Orientation = StyleCDO->Orientation;
		SelectionMode = StyleCDO->SelectionMode;
	}

	Super::SynchronizeProperties();
}

TSharedRef<STableViewBase> UModioCommonTileView::RebuildListWidget()
{
	return ConstructTileView<SModioCommonTileView>();
}

UUserWidget& UModioCommonTileView::OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable)
{
	if (DesiredEntryClass->IsChildOf<UCommonButtonBase>())
	{
		return GenerateTypedEntry<UUserWidget, SModioCommonButtonTableRow<UObject*>>(DesiredEntryClass, OwnerTable);
	}
	return GenerateTypedEntry(DesiredEntryClass, OwnerTable);
}
