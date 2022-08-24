// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/CommonComponents/ModioDrawer.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Core/ModioUIHelpers.h"
#include "Math/UnrealMathUtility.h"
#include "Misc/EngineVersionComparison.h"
#include "UI/BaseWidgets/ModioGridPanel.h"
#include "UI/Styles/ModioDrawerWidgetStyle.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SGridPanel.h"

const FName UModioDrawer::HeaderName = FName("Header");
const FName UModioDrawer::ContentName = FName("Content");
const FName UModioDrawer::FooterName = FName("Footer");

void UModioDrawer::GetSlotNames(TArray<FName>& SlotNames) const
{
	SlotNames.Add(HeaderName);
	SlotNames.Add(ContentName);
	SlotNames.Add(FooterName);
}

UWidget* UModioDrawer::GetContentForSlot(FName SlotName) const
{
	if (NamedWidgets.Contains(SlotName))
	{
		return NamedWidgets[SlotName];
	}
	return nullptr;
}

void UModioDrawer::SetContentForSlot(FName SlotName, UWidget* Content)
{
	if (SlotName == ContentName || SlotName == FooterName || SlotName == HeaderName)
	{
		NamedWidgets.Add(SlotName, Content);
	}
	if (Content == nullptr)
	{
		NamedWidgets.Remove(SlotName);
	}
}

TSharedRef<SWidget> UModioDrawer::GetHeaderWidget()
{
	TSharedRef<SWidget> ActualHeaderContent =
		NamedWidgets.Contains(HeaderName) ? NamedWidgets[HeaderName]->TakeWidget() : SNullWidget::NullWidget;

	if (const FModioDrawerWidgetStyle* Style = DrawerStyle.FindStyle<FModioDrawerWidgetStyle>())
	{
		return SNew(SBorder).BorderImage(&Style->HeadingBackgroundBrush).Padding(HeaderPadding)[ActualHeaderContent];
	}

	// If we don't have a style set, presume that the content widget doesn't have a border/wrapper

	return ActualHeaderContent;
}

TSharedRef<SWidget> UModioDrawer::GetContentWidget()
{
	TSharedRef<SWidget> ActualContentContent =
		NamedWidgets.Contains(ContentName) ? NamedWidgets[ContentName]->TakeWidget() : SNullWidget::NullWidget;

	if (const FModioDrawerWidgetStyle* Style = DrawerStyle.FindStyle<FModioDrawerWidgetStyle>())
	{
		return SNew(SBorder).BorderImage(&Style->ContentBackgroundBrush).Padding(ContentPadding)[ActualContentContent];
	}

	// If we don't have a style set, presume that the content widget doesn't have a border/wrapper

	return ActualContentContent;
}

TSharedRef<SWidget> UModioDrawer::GetFooterWidget()
{
	TSharedRef<SWidget> ActualFooterContent =
		NamedWidgets.Contains(FooterName) ? NamedWidgets[FooterName]->TakeWidget() : SNullWidget::NullWidget;

	if (const FModioDrawerWidgetStyle* Style = DrawerStyle.FindStyle<FModioDrawerWidgetStyle>())
	{
		return SNew(SBorder).BorderImage(&Style->FooterBackgroundBrush).Padding(FooterPadding)[ActualFooterContent];
	}

	// If we don't have a style set, presume that the content widget doesn't have a border/wrapper

	return ActualFooterContent;
}

void UModioDrawer::ReleaseSlateResources(bool bReleaseChildren)
{
	MyBorder.Reset();
	MyGridPanel.Reset();
	MyDrawerBox.Reset();
	for (auto NamedWidget : NamedWidgets)
	{
		if (NamedWidget.Value)
		{
			NamedWidget.Value->ReleaseSlateResources(bReleaseChildren);
		}
	}
	NamedWidgets.Empty();
	UWidget::ReleaseSlateResources(bReleaseChildren);
}

TSharedRef<SWidget> UModioDrawer::RebuildWidget()
{
	// clang-format off
	return SAssignNew(MyBorder, SBorder)
	.Padding(0)
	.OnMouseButtonUp_UObject(this, &UModioDrawer::OnMouseUp)
	.Content()
		[
			SAssignNew(MyDrawerBox, SBox)
			.MinDesiredWidth(MinWidth)
			[
				SAssignNew(MyGridPanel, SGridPanel)
				.FillRow(0, HeaderFillOverride)
				.FillRow(1, ContentFillOverride)
				.FillRow(2, FooterFillOverride)
				.FillColumn(0, 1)
				+SGridPanel::Slot(0, 0)
				.HAlign(HeaderHorizontalAlignment)
				.VAlign(HeaderVerticalAlignment)
				[
					GetHeaderWidget()
				]
				+SGridPanel::Slot(0,1)
				.HAlign(ContentHorizontalAlignment)
				.VAlign(ContentVerticalAlignment)
				[
					GetContentWidget()
				]
				+SGridPanel::Slot(0,2)
				.HAlign(FooterHorizontalAlignment)
				.VAlign(FooterVerticalAlignment)
				[
					GetFooterWidget()
				]
			]
		];
	// clang-format on
}

void UModioDrawer::SynchronizeProperties()
{
	UWidget::SynchronizeProperties();
	MyGridPanel->ClearChildren();
	MyGridPanel->SetColumnFill(0, 1);

	MyGridPanel->SetRowFill(0, HeaderFillOverride);
	MyGridPanel->SetRowFill(1, ContentFillOverride);
	MyGridPanel->SetRowFill(2, FooterFillOverride);

	const FModioDrawerWidgetStyle* Style = DrawerStyle.FindStyle<FModioDrawerWidgetStyle>();
	SGridPanel::FSlot& HeaderSlot = *ModioUIHelpers::AddSlot(MyGridPanel.Get(), 0, 0);
	ModioUIHelpers::SetHorizontalAlignment(HeaderSlot, HeaderHorizontalAlignment);
	ModioUIHelpers::SetVerticalAlignment(HeaderSlot, HeaderVerticalAlignment);
	HeaderSlot.AttachWidget(GetHeaderWidget());

	SGridPanel::FSlot& ContentSlot = *ModioUIHelpers::AddSlot(MyGridPanel.Get(), 0, 1);
	ModioUIHelpers::SetHorizontalAlignment(ContentSlot, ContentHorizontalAlignment);
	ModioUIHelpers::SetVerticalAlignment(ContentSlot, ContentVerticalAlignment);
	ContentSlot.AttachWidget(GetContentWidget());
	
	SGridPanel::FSlot& FooterSlot = *ModioUIHelpers::AddSlot(MyGridPanel.Get(), 0, 2);
	ModioUIHelpers::SetHorizontalAlignment(FooterSlot, FooterHorizontalAlignment);
	ModioUIHelpers::SetVerticalAlignment(FooterSlot, FooterVerticalAlignment);
	FooterSlot.AttachWidget(GetFooterWidget());

	MyGridPanel->Invalidate(EInvalidateWidgetReason::Layout);
}

FReply UModioDrawer::OnMouseUp(const FGeometry&, const FPointerEvent&)
{
	return FReply::Handled();
}
