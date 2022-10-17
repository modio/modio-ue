/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

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

void IModioUIPopupMenuContentWidget::SetStyle_Implementation(FModioUIStyleRef StyleRef) 
{
	NativeSetStyle(StyleRef);
}