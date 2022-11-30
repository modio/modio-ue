/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/BaseWidgets/ModioPopupMenu.h"
#include "Core/Input/ModioInputKeys.h"
#include "UI/BaseWidgets/ModioDefaultPopupMenuContent.h"
#include "UI/CommonComponents/ModioRichTextButton.h"
#include "UI/Interfaces/IModioUIPopupMenuContentWidget.h"
#include "UI/Styles/ModioPopupMenuStyle.h"
#include "Widgets/Input/SMenuAnchor.h"

void UModioPopupMenu::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	OnGetUserMenuContentEvent.BindDynamic(this, &UModioPopupMenu::GeneratePopupMenuContent);
}

UUserWidget* UModioPopupMenu::GeneratePopupMenuContent()
{
	// Currently we only use the width so that the popup has the same width as the menu anchor itself
	FGeometry PaintGeo = GetPaintSpaceGeometry();
	FVector2D ActualSize = PaintGeo.GetLocalSize();
	if (MenuContentWidgetClass)
	{
		UUserWidget* ConcreteWidget = CreateWidget<UUserWidget>(this, MenuContentWidgetClass.Get());
		ConcreteWidget->TakeWidget();
		if (ConcreteWidget->Implements<UModioUIPopupMenuContentWidget>())
		{
			IModioUIPopupMenuContentWidget::Execute_SetMenuEntries(ConcreteWidget, CurrentEntries);
			IModioUIPopupMenuContentWidget::Execute_SetDesiredSize(ConcreteWidget, ActualSize);
			IModioUIPopupMenuContentWidget::Execute_SetStyle(ConcreteWidget, PopupMenuStyle);
			
			if (IModioUIPopupMenuContentWidget* Interface = Cast<IModioUIPopupMenuContentWidget>(ConcreteWidget))
			{
				Interface->GetContentCloseDelegate().BindUObject(this, &UModioPopupMenu::OnContentClose);
			}
		}

		return ConcreteWidget;
	}
	else
	{
		UUserWidget* ConcreteWidget = CreateWidget<UModioDefaultPopupMenuContent>(this);
		ConcreteWidget->TakeWidget();
		if (ConcreteWidget->Implements<UModioUIPopupMenuContentWidget>())
		{
			IModioUIPopupMenuContentWidget::Execute_SetMenuEntries(ConcreteWidget, CurrentEntries);
			IModioUIPopupMenuContentWidget::Execute_SetDesiredSize(ConcreteWidget, ActualSize);
			IModioUIPopupMenuContentWidget::Execute_SetStyle(ConcreteWidget, PopupMenuStyle);

			// This below function isn't part of the UInterface so doesn't use the Execute_ method
			if (IModioUIPopupMenuContentWidget* Interface = Cast<IModioUIPopupMenuContentWidget>(ConcreteWidget))
			{
				Interface->GetContentCloseDelegate().BindUObject(this, &UModioPopupMenu::OnContentClose);
			}
		}

		return ConcreteWidget;
	}
}

TSharedRef<SWidget> UModioPopupMenu::RebuildWidget()
{
	MenuButton = NewObject<UModioRichTextButton>();
	MenuButton->SetButtonStyle(ButtonStyle, true);
	MenuButton->SetJustification(ButtonLabelJustification);
	MenuButton->SetLabel(ButtonLabel);
	MenuButton->OnClicked.AddDynamic(this, &UModioPopupMenu::HandleButtonClicked);
	// Refactor and expose on the popup menu?
	MenuButton->DisplayHintForInputHandler(FModioInputKeys::MoreOptions);
	TSharedRef<SWidget> Widget = SAssignNew(MyMenuAnchor, SMenuAnchor)
									 .Placement(Placement)
									 .FitInWindow(bFitInWindow)
									 .Method(EPopupMethod::UseCurrentWindow)
									 .OnGetMenuContent(BIND_UOBJECT_DELEGATE(FOnGetContent, HandleGetMenuContent))
									 .OnMenuOpenChanged(BIND_UOBJECT_DELEGATE(FOnIsOpenChanged, HandleMenuOpenChanged))
									 .ShouldDeferPaintingAfterWindowContent(ShouldDeferPaintingAfterWindowContent)
									 .UseApplicationMenuStack(UseApplicationMenuStack)[MenuButton->TakeWidget()];
	return Widget;
}

void UModioPopupMenu::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	MenuButton = nullptr;
}

void UModioPopupMenu::HandleButtonClicked()
{
	ToggleOpen(true);
}

void UModioPopupMenu::OnContentClose()
{
	Close();
}

void UModioPopupMenu::SetMenuEntries(FModioUIMenuCommandList Entries)
{
	CurrentEntries = Entries;
}
