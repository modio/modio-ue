// Fill out your copyright notice in the Description page of Project Settings.

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
