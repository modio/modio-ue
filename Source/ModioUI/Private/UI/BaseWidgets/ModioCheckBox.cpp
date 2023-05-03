/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/BaseWidgets/ModioCheckBox.h"
#include "Settings/ModioUISettings.h"
#include "Slate/SRetainerWidget.h"
#include "Templates/SharedPointer.h"
#include "UI/BaseWidgets/Slate/SModioRichTextBlock.h"
#include "UI/Styles/ModioCheckBoxStyle.h"
#include "UI/Styles/ModioUIStyleSet.h"
#include "UI/Styles/ModioWidgetBorderStyle.h"
#include "Widgets/Input/SCheckBox.h"

void UModioCheckBox::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	MyCheckbox->SetContent(GetContentWidget());
	MyCheckbox->SetStyle(GetCheckboxStyle());
	if (MyDefaultLabel.IsValid())
	{
		MyDefaultLabel->SetText(
			TAttribute<FText>::Create(TAttribute<FText>::FGetter::CreateUObject(this, &UModioCheckBox::GetLabelText)));
	}
}

const FCheckBoxStyle* UModioCheckBox::GetCheckboxStyle() const
{
	if (const FModioCheckBoxStyle* ResolvedStyle = CheckBoxStyle.FindStyle<FModioCheckBoxStyle>())
	{
		return &ResolvedStyle->CheckBoxStyle;
	}
	else
	{
		return &WidgetStyle;
	}
}

const FModioUIStyleRef* UModioCheckBox::GetLabelStyleRef() const
{
	static FModioUIStyleRef DefaultStyleRef = {"DefaultRichTextStyle"};
	if (const FModioCheckBoxStyle* ResolvedStyle = CheckBoxStyle.FindStyle<FModioCheckBoxStyle>())
	{
		return &ResolvedStyle->TextStyleNew;
	}
	return &DefaultStyleRef;
}

FText UModioCheckBox::GetLabelText() const
{
	return LabelText;
}

TSharedRef<SWidget> UModioCheckBox::GetContentWidget()
{
	MyDefaultLabel.Reset();

	if (GetContentSlot() && GetContentSlot()->Content)
	{
		return GetContentSlot()->Content->TakeWidget();
	}
	else if (!LabelText.IsEmpty())
	{
		return SAssignNew(MyDefaultLabel, SModioRichTextBlock)
			.StyleReference_UObject(this, &UModioCheckBox::GetLabelStyleRef)
			.Text(TAttribute<FText>::Create(
				TAttribute<FText>::FGetter::CreateUObject(this, &UModioCheckBox::GetLabelText)));
	}
	else
	{
		return SNullWidget::NullWidget;
	}
}
FMargin UModioCheckBox::GetCheckboxContentPadding() const
{

	if (const FModioCheckBoxStyle* ResolvedStyle = CheckBoxStyle.FindStyle<FModioCheckBoxStyle>())
	{
	
		return ResolvedStyle->ContentPadding;
	}
	else
	{
		return FMargin {};
	}
}

TSharedRef<SWidget> UModioCheckBox::RebuildWidget()
{
	const FCheckBoxStyle* style = GetCheckboxStyle();
	FMargin margin = GetCheckboxContentPadding();
	return SAssignNew(MyCheckbox, SCheckBox)
		.OnCheckStateChanged(BIND_UOBJECT_DELEGATE(FOnCheckStateChanged, SlateOnCheckStateChangedCallback))
		.Style(style)
		.HAlign(HorizontalAlignment)
		.IsFocusable(IsFocusable)
		.Padding(margin)[GetContentWidget()];
}

void UModioCheckBox::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	MyCheckbox.Reset();
	MyDefaultLabel.Reset();
}

void UModioCheckBox::SetLabelText(FText InText)
{
	LabelText = InText;
	if (MyCheckbox)
	{
		MyCheckbox->Invalidate(EInvalidateWidgetReason::Layout);
	}
}

void UModioCheckBox::SetStyle(const FModioCheckBoxStyle* InStyle) 
{
	MyCheckbox->SetStyle(&InStyle->CheckBoxStyle);
	MyDefaultLabel->SetTextStyle(InStyle->TextStyle);
}
