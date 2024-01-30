/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Foundation/Components/Text/RichTextBlock/ModioCommonRichTextBlock.h"

#include "CommonLazyImage.h"
#include "UI/Foundation/Components/Text/RichTextBlock/ModioCommonRichTextStyle.h"
#include "Widgets/Images/SImage.h"

void UModioCommonRichTextBlock::SetStyle(TSubclassOf<UModioCommonRichTextStyle> InStyle)
{
	if (InStyle && InStyle != ModioStyle)
	{
		ModioStyle = InStyle;
		SynchronizeProperties();
	}
}

void UModioCommonRichTextBlock::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	UModioCommonRichTextStyle* StyleCDO = ModioStyle.GetDefaultObject();
	if (!StyleCDO)
	{
		return;
	}

	RefreshContent();
	FTextBlockStyle DefaultStyle;
	StyleCDO->ToTextBlockStyle(DefaultStyle);
	SetDefaultTextStyle(DefaultStyle);
}

void UModioCommonRichTextBlock::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	TextImagePanel.Reset();
	TextWidget.Reset();
	ImageWidget.Reset();
	ImageBrush.Reset();
	TextImageOrder.Reset();
}

TSharedRef<SWidget> UModioCommonRichTextBlock::RebuildWidget()
{
	TextImagePanel = SNew(SHorizontalBox);
	TextWidget = Super::RebuildWidget();
	return RefreshContent();
}

TSharedRef<SWidget> UModioCommonRichTextBlock::RefreshContent()
{
	if (!TextWidget.IsValid())
	{
		return SNullWidget::NullWidget;
	}

	if (!TextImagePanel.IsValid())
	{
		return TextWidget.ToSharedRef();
	}

	UModioCommonRichTextStyle* StyleCDO = ModioStyle.GetDefaultObject();

	// clang-format off
	auto AddImageWidgetToPanel = [this, &StyleCDO]()
	{
		if (ImageWidget.IsValid())
		{
			TextImagePanel->AddSlot()
				.Expose(ImageSlot)
				.SizeParam(StyleCDO ? UWidget::ConvertSerializedSizeParamToRuntime(StyleCDO->ImageSize) : FStretch(0.0f))
				.VAlign(StyleCDO ? StyleCDO->ImageVerticalAlignment.GetValue() : EVerticalAlignment::VAlign_Fill)
				.HAlign(StyleCDO ? StyleCDO->ImageHorizontalAlignment.GetValue() : EHorizontalAlignment::HAlign_Fill)
				.Padding(StyleCDO ? StyleCDO->ImagePadding : FMargin(0.0f))
				[
					ImageWidget.ToSharedRef()
				];
		}
	};

	auto AddTextWidgetToPanel = [this, &StyleCDO]()
	{
		if (TextWidget.IsValid())
		{
			TextImagePanel->AddSlot()
				.Expose(TextSlot)
				.SizeParam(StyleCDO ? UWidget::ConvertSerializedSizeParamToRuntime(StyleCDO->TextSize) : FStretch(1.0f))
				.VAlign(StyleCDO ? StyleCDO->TextVerticalAlignment.GetValue() : EVerticalAlignment::VAlign_Fill)
				.HAlign(StyleCDO ? StyleCDO->TextHorizontalAlignment.GetValue() : EHorizontalAlignment::HAlign_Fill)
				.Padding(StyleCDO ? StyleCDO->TextPadding : FMargin(0.0f))
				[
					TextWidget.ToSharedRef()
				];
		}
	};
	// clang-format off

	if (StyleCDO)
	{
		if (!ImageWidget || !ImageBrush.IsSet() || ImageBrush != StyleCDO->ImageBrush)
		{
			ImageBrush = StyleCDO->ImageBrush;
			ImageWidget = SNew(SImage)
					.FlipForRightToLeftFlowDirection(false)
					.Image(&ImageBrush.GetValue());
			ImageWidget->SetVisibility(ImageBrush.GetValue().GetResourceObject() && ImageBrush.GetValue().DrawAs != ESlateBrushDrawType::NoDrawType ? EVisibility::Visible : EVisibility::Collapsed);
		}

		if (!TextSlot || !TextImageOrder.IsSet() || TextImageOrder.GetValue() != StyleCDO->TextImageOrder)
		{
			TextImagePanel->ClearChildren();
			TextImageOrder = StyleCDO->TextImageOrder;
			if (TextImageOrder == EModioCommonRichTextStyleTextImageOrder::ImageFirst)
			{
				AddImageWidgetToPanel();
				AddTextWidgetToPanel();
			}
			else if (TextImageOrder == EModioCommonRichTextStyleTextImageOrder::TextFirst)
			{
				AddTextWidgetToPanel();
				AddImageWidgetToPanel();
			}
		}
	}
	else if (!TextSlot)
	{
		TextImagePanel->ClearChildren();
		AddTextWidgetToPanel();
	}
	
	return TextImagePanel.ToSharedRef();
}
