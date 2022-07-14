// Fill out your copyright notice in the Description page of Project Settings.

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
