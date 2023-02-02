/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/CommonComponents/ModioDrawerContentBase.h"
#include "Blueprint/WidgetTree.h"
#include "Components/NamedSlot.h"
#include "Components/VerticalBoxSlot.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Layout/SBox.h"

void UModioDrawerContentBase::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	HeaderBackgroundImage.Reset();
	ContentBackgroundImage.Reset();
	OptionalContentBackgroundImage.Reset();
	if (HeaderSlot)
	{
		HeaderSlot->ReleaseSlateResources(bReleaseChildren);
	}
	if (ContentSlot)
	{
		ContentSlot->ReleaseSlateResources(bReleaseChildren);
	}
	if (OptionalContentSlot)
	{
		OptionalContentSlot->ReleaseSlateResources(bReleaseChildren);
	}
}

TSharedRef<SWidget> UModioDrawerContentBase::RebuildWidget()
{
	// clang-format off
	return SAssignNew(MyVerticalBox, SVerticalBox)
	+ SVerticalBox::Slot()
	[
		SNew(SOverlay) 
		+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SAssignNew(HeaderBackgroundImage, SImage)
		]
		+ SOverlay::Slot()
		[
			NativeGetHeaderWidget()
		]
	]
	+ SVerticalBox::Slot()
	[
		SNew(SOverlay) 
		+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SAssignNew(ContentBackgroundImage, SImage)
		]
		+ SOverlay::Slot()
		[
			NativeGetContentWidget()
		]
	]
	+ SVerticalBox::Slot()
	[
		SNew(SOverlay) 
		+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SAssignNew(OptionalContentBackgroundImage, SImage)
		]
		+ SOverlay::Slot()
		[
			NativeGetOptionalContentWidget()
		]
	];
	// clang-format on
}

void UModioDrawerContentBase::SynchronizeProperties()
{
	Super::SynchronizeProperties();
}

void UModioDrawerContentBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

TSharedRef<SWidget> UModioDrawerContentBase::NativeGetHeaderWidget()
{
	UWidget* WidgetFromBP = GetHeaderWidget();
	return WidgetFromBP ? WidgetFromBP->TakeWidget() : SNullWidget::NullWidget;
}

TSharedRef<SWidget> UModioDrawerContentBase::NativeGetContentWidget()
{
	UWidget* WidgetFromBP = GetContentWidget();
	return WidgetFromBP ? WidgetFromBP->TakeWidget() : SNullWidget::NullWidget;
}

TSharedRef<SWidget> UModioDrawerContentBase::NativeGetOptionalContentWidget()
{
	UWidget* WidgetFromBP = GetOptionalContentWidget();
	return WidgetFromBP ? WidgetFromBP->TakeWidget() : SNullWidget::NullWidget;
}
