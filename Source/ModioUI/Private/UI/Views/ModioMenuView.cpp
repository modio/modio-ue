/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/Views/ModioMenuView.h"
#include "Widgets/SNullWidget.h"

const FText& UModioMenuView::GetName()
{
	return MenuName;
}

TSharedRef<SWidget> UModioMenuView::GetMenuTitleContent()
{
	if (TitleContentClass)
	{
		CachedMenuTitleContentWidget = CreateWidget<UUserWidget>(this, TitleContentClass);
		return CachedMenuTitleContentWidget->TakeWidget();
	}

	return SNullWidget::NullWidget;
}

bool UModioMenuView::ShouldShowSearchButtonForMenu()
{
	return false;
}
