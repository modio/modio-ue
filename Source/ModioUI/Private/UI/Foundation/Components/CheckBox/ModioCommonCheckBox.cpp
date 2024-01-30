/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Foundation/Components/CheckBox/ModioCommonCheckBox.h"

#include "UI/Foundation/Components/CheckBox/ModioCommonCheckBoxStyle.h"
#include "UI/Foundation/Components/Text/TextBlock/ModioCommonTextBlock.h"
#include "Widgets/Input/SCheckBox.h"
#include "Misc/EngineVersionComparison.h"

class SModioCommonCheckBox : public SCheckBox
{
	virtual void OnMouseEnter( const FGeometry& MyGeometry, const FPointerEvent& MouseEvent ) override
	{
		OnHoverChanged.Broadcast(true);
		return SCheckBox::OnMouseEnter(MyGeometry, MouseEvent);
	}

	virtual void OnMouseLeave( const FPointerEvent& MouseEvent ) override
	{
		OnHoverChanged.Broadcast(false);
		return SCheckBox::OnMouseLeave(MouseEvent);
	}

	virtual FReply OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent) override
	{
		OnHoverChanged.Broadcast(true);
		return SCheckBox::OnFocusReceived(MyGeometry, InFocusEvent);
	}

	virtual void OnFocusLost(const FFocusEvent& InFocusEvent) override
	{
		OnHoverChanged.Broadcast(false);
		return SCheckBox::OnFocusLost(InFocusEvent);
	}

public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnHoverChanged, bool);
	FOnHoverChanged OnHoverChanged;
};

UModioCommonCheckBox::UModioCommonCheckBox()
{
	OnCheckStateChanged.AddDynamic(this, &UModioCommonCheckBox::HandleOnCheckStateChanged);
}

void UModioCommonCheckBox::SetStyle(TSubclassOf<UModioCommonCheckBoxStyle> InStyle)
{
	if (InStyle && InStyle != ModioStyle)
	{
		ModioStyle = InStyle;
		if (IsConstructed())
		{
			SynchronizeProperties();
		}
	}
}

void UModioCommonCheckBox::SetLabel(const FText& InLabelText)
{
	LabelText = InLabelText;
	if (MyCheckbox)
	{
		MyCheckbox->Invalidate(EInvalidateWidgetReason::Layout);
	}
}

FText UModioCommonCheckBox::GetLabel() const
{
	return LabelText;
}

void UModioCommonCheckBox::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (MyCheckbox && ModioStyle)
	{
		UModioCommonCheckBoxStyle* StyleCDO = ModioStyle->GetDefaultObject<UModioCommonCheckBoxStyle>();
		MyCheckbox->SetStyle(&StyleCDO->WidgetStyle);
		MyCheckbox->SetContent(GetContentWidget());
	}

	if (LabelTextBlock)
	{
		LabelTextBlock->SetText(LabelText);
	}
}

FText UModioCommonCheckBox::GetLabelTextBlockText() const
{
	return LabelText;
}

TSharedRef<SWidget> UModioCommonCheckBox::GetContentWidget()
{
	LabelTextBlock = nullptr;

	if (GetContentSlot() && GetContentSlot()->Content)
	{
		return GetContentSlot()->Content->TakeWidget();
	}
	else if (!LabelText.IsEmpty())
	{
		LabelTextBlock = NewObject<UModioCommonTextBlock>(this);
		LabelTextBlock->SetText(LabelText);

		if (UModioCommonCheckBoxStyle* StyleCDO = Cast<UModioCommonCheckBoxStyle>(ModioStyle.GetDefaultObject()))
		{
			LabelTextBlock->SetStyle(StyleCDO->LabelTextStyle);
		}

		return LabelTextBlock->TakeWidget();
	}
	else
	{
		return SNullWidget::NullWidget;
	}
}

TSharedRef<SWidget> UModioCommonCheckBox::RebuildWidget()
{
	if (!ModioStyle)
	{
		return Super::RebuildWidget();
	}

	UModioCommonCheckBoxStyle* StyleCDO = ModioStyle->GetDefaultObject<UModioCommonCheckBoxStyle>();
	TSharedRef<SWidget> RebuiltCheckBox = SAssignNew(MyCheckbox, SModioCommonCheckBox)
		.OnCheckStateChanged(BIND_UOBJECT_DELEGATE(FOnCheckStateChanged, SlateOnCheckStateChangedCallback))
		.Style(&StyleCDO->WidgetStyle)
		.HAlign(HorizontalAlignment)
		.IsFocusable(
#if UE_VERSION_NEWER_THAN(5, 2, 0)
		GetIsFocusable()
#else
		IsFocusable
#endif
		)
		.Padding_UObject(this, &UModioCommonCheckBox::GetCheckboxContentPadding)[GetContentWidget()];

	if (SModioCommonCheckBox* ModioCommonCheckBox = StaticCast<SModioCommonCheckBox*>(MyCheckbox.Get()))
	{
		ModioCommonCheckBox->OnHoverChanged.AddWeakLambda(this, [this](bool bHovered) {
			if (UModioCommonCheckBoxStyle* StyleCDO = Cast<UModioCommonCheckBoxStyle>(ModioStyle.GetDefaultObject()))
			{
				if (LabelTextBlock)
				{
					LabelTextBlock->SetStyle(bHovered ? StyleCDO->HoveredLabelTextStyle : StyleCDO->LabelTextStyle);
				}
			}
		});
	}

	return RebuiltCheckBox;
}

void UModioCommonCheckBox::ReleaseSlateResources(bool bReleaseChildren)
{
	LabelTextBlock = nullptr;
	Super::ReleaseSlateResources(bReleaseChildren);
}

FMargin UModioCommonCheckBox::GetCheckboxContentPadding_Implementation() const
{
	if (!ModioStyle)
	{
		return FMargin();
	}

	UModioCommonCheckBoxStyle* StyleCDO = ModioStyle->GetDefaultObject<UModioCommonCheckBoxStyle>();
	return StyleCDO->ContentPadding;
}

void UModioCommonCheckBox::HandleOnCheckStateChanged_Implementation(bool bIsChecked)
{
	if (OnModioCommonCheckBoxStateChanged.IsBound())
	{
		OnModioCommonCheckBoxStateChanged.Broadcast(bIsChecked);
	}
}
