#include "UI/CommonComponents/ModioDownloadListWidgetBase.h"

void UModioDownloadListWidgetBase::NativeOnSetDataSource()
{
	Super::NativeOnSetDataSource();
	Refresh();
}

void UModioDownloadListWidgetBase::Refresh()
{
	
}

void UModioDownloadListWidgetBase::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	SetDataSource(ListItemObject);
}