/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Foundation/Components/ExpandableArea/ModioCommonExpandableArea.h"

#include "Components/PanelWidget.h"
#include "UI/Foundation/Components/ExpandableArea/ModioCommonExpandableAreaStyle.h"
#include "UI/Foundation/Components/Text/TextBlock/ModioCommonTextBlock.h"
#include "UI/Foundation/Components/Text/TextBlock/ModioCommonTextBlockStyle.h"
#include "Widgets/Layout/SExpandableArea.h"
#include "Algo/Find.h"
#include "Misc/EngineVersionComparison.h"

void UModioCommonExpandableArea::SetStyle(TSubclassOf<UModioCommonExpandableAreaStyle> InStyle)
{
	if (InStyle && InStyle != ModioStyle)
	{
		ModioStyle = InStyle;
		SynchronizeProperties();
	}
}

void UModioCommonExpandableArea::HandleOnFocusReceived()
{
	bIsFocused = true;
	RefreshStyle();
}

void UModioCommonExpandableArea::HandleOnFocusLost()
{
	bIsFocused = false;
	RefreshStyle();
}

void UModioCommonExpandableArea::SynchronizeProperties()
{
	if (!CachedAppliedModioStyle || ModioStyle != CachedAppliedModioStyle)
	{
		RefreshStyle();
		RefreshContent();
	}
	Super::SynchronizeProperties();
}

TSharedRef<SWidget> UModioCommonExpandableArea::RebuildWidget()
{
	WrapperWidget = SNew(SOverlay);
	return RefreshContent();
}

void UModioCommonExpandableArea::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	WrapperWidget.Reset();
	CachedAppliedModioStyle = nullptr;
}

bool FindTextBlockInPanel(UPanelWidget* PanelWidget, UModioCommonTextBlock*& OutTextBlock)
{
	TArray<UWidget*> AllChildren = PanelWidget->GetAllChildren();
	for (UWidget* Child : AllChildren)
	{
		if (UModioCommonTextBlock* TextBlock = Cast<UModioCommonTextBlock>(Child))
		{
			OutTextBlock = TextBlock;
			return true;
		}

		if (UPanelWidget* ChildPanel = Cast<UPanelWidget>(Child))
		{
			if (FindTextBlockInPanel(ChildPanel, OutTextBlock))
			{
				return true;
			}
		}
	}

	return false;
}

bool UModioCommonExpandableArea::GetAttachedLabelTextBlock(UModioCommonTextBlock*& AttachedTextBlock)
{
	// If the header content is directly a text block, use that
	if (UModioCommonTextBlock* TextBlock = Cast<UModioCommonTextBlock>(HeaderContent))
	{
		AttachedTextBlock = TextBlock;
		return true;
	}

	// If the header content is a panel, search for a text block in the children
	if (UPanelWidget* PanelWidget = Cast<UPanelWidget>(HeaderContent))
	{
		if (FindTextBlockInPanel(PanelWidget, AttachedTextBlock))
		{
			return true;
		}
	}

	return false;
}

TSharedRef<SWidget> UModioCommonExpandableArea::RefreshContent()
{
	TSharedRef<SWidget> ParentWidget = Super::RebuildWidget();

	if (!MyExpandableArea.IsValid())
	{
		return ParentWidget;
	}

	if (!WrapperWidget)
	{
		return MyExpandableArea.ToSharedRef();
	}

	// clang-format off
	WrapperWidget->ClearChildren();
	WrapperWidget->AddSlot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.Padding(0)
		[
			ParentWidget
		];
	// clang-format off

	return WrapperWidget.ToSharedRef();
}

void UModioCommonExpandableArea::RefreshStyle()
{
	if (UModioCommonExpandableAreaStyle* StyleCDO = ModioStyle.GetDefaultObject())
	{
#if UE_VERSION_NEWER_THAN(5, 2, 0)
		Super::SetStyle(StyleCDO->Style);
		SetBorderBrush(StyleCDO->BorderBrush);
		SetBorderColor(StyleCDO->BorderColor);
PRAGMA_DISABLE_DEPRECATION_WARNINGS
		bIsExpanded = StyleCDO->bIsExpandedByDefault;
PRAGMA_ENABLE_DEPRECATION_WARNINGS
		SetMaxHeight(StyleCDO->MaxHeight);
		SetHeaderPadding(StyleCDO->HeaderPadding);
		SetAreaPadding(StyleCDO->AreaPadding);
#else
		Style = StyleCDO->Style;
		BorderBrush = StyleCDO->BorderBrush;
		BorderColor = StyleCDO->BorderColor;
		bIsExpanded = StyleCDO->bIsExpandedByDefault;
		MaxHeight = StyleCDO->MaxHeight;
		HeaderPadding = StyleCDO->HeaderPadding;
		AreaPadding = StyleCDO->AreaPadding;
#endif

		TSubclassOf<UModioCommonTextStyle> LabelTextStyle = bIsFocused ? StyleCDO->SelectedLabelTextStyle : StyleCDO->NormalLabelTextStyle;

		if (LabelTextStyle && HeaderContent)
		{
			UModioCommonTextBlock* TextBlock;
			if (GetAttachedLabelTextBlock(TextBlock))
			{
				TextBlock->SetStyle(LabelTextStyle);
			}
		}
	}
}
