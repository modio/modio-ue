/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/CommonComponents/ModioTabButton.h"
#include "UI/Views/ModioMenuView.h"

void UModioTabButton::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	SetDataSource(ListItemObject);
}

void UModioTabButton::NativeOnSetDataSource()
{
	Super::NativeOnSetDataSource();
	// Handled here rather than in SynchronizeProperties, because in the UMG editor window there are no list items and
	// therefore no data sources
	if (ClickableRegion)
	{
		UModioMenuView* View = Cast<UModioMenuView>(DataSource);
		if (View)
		{
			ClickableRegion->SetLabel(View->GetName());
		}
	}
}
