#include "UI/Interfaces/IModioUIPopupMenuContentWidget.h"

void IModioUIPopupMenuContentWidget::SetMenuEntries_Implementation(const FModioUIMenuCommandList& Entries)
{
	NativeSetMenuEntries(Entries);
}

void IModioUIPopupMenuContentWidget::SetDesiredSize_Implementation(const FVector2D& DesiredSize) 
{
	NativeSetDesiredSize(DesiredSize);
}

FOnContentClose& IModioUIPopupMenuContentWidget::GetContentCloseDelegate()
{
	return NativeGetContentCloseDelegate();
}
