/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Foundation/Components/Button/ModioCommonButtonBase.h"
#include "CommonActionWidget.h"
#include "Input/CommonUIInputTypes.h"
#include "UI/Foundation/Components/Text/TextBlock/ModioCommonTextBlock.h"
#include "UI/Foundation/Components/Image/ModioCommonDynamicImage.h"

void UModioCommonButtonBase::SetLabel(FText NewLabel)
{
	ButtonText = NewLabel;
	if (TextBlock)
	{
		TextBlock->SetText(ButtonText);
	}
}

void UModioCommonButtonBase::SetStyle(TSubclassOf<UModioCommonButtonStyle> InStyle)
{
	ModioDefaultStyleClass = InStyle;
	TSubclassOf<UCommonButtonStyle> CommonStyle = InStyle;
	Super::SetStyle(CommonStyle);
	SynchronizeProperties();
}

bool UModioCommonButtonBase::IsButtonFocused() const
{
	return HasAnyUserFocus() || HasFocusedDescendants();
}

void UModioCommonButtonBase::BindTriggeringInputActionToClick()
{
	if (TriggeringInputAction.IsNull() || !TriggeredInputAction.IsNull())
	{
		return;
	}

	if (!TriggeringBindingHandle.IsValid())
	{
		FBindUIActionArgs BindArgs(TriggeringInputAction, false, FSimpleDelegate::CreateWeakLambda(this, [this](){ HandleButtonClicked(); }));
		BindArgs.OnHoldActionProgressed.BindWeakLambda(this, [this](float Progress){ NativeOnActionProgress(Progress); });
		if (!OverrideActionDisplayName.IsEmpty())
		{
			BindArgs.OverrideDisplayName = OverrideActionDisplayName;
		}
		else if (!ButtonText.IsEmpty())
		{
			BindArgs.OverrideDisplayName = ButtonText;
		}

		TriggeringBindingHandle = RegisterUIActionBinding(BindArgs);
	}
}

void UModioCommonButtonBase::NativeOnInitialized()
{
	if (Style && Style->IsChildOf(UModioCommonButtonStyle::StaticClass()))
	{
		ModioDefaultStyleClass = Style;
	}
	
	Super::NativeOnInitialized();
}

void UModioCommonButtonBase::NativeOnCurrentTextStyleChanged()
{
	Super::NativeOnCurrentTextStyleChanged();
	if (TextBlock)
	{
		Cast<UCommonTextBlock>(TextBlock)->SetStyle(GetCurrentTextStyleClass());
	}
}

bool UModioCommonButtonBase::Initialize()
{
	return Super::Initialize();
}

void UModioCommonButtonBase::NativePreConstruct()
{
	SetLabel(ButtonText);
}

void UModioCommonButtonBase::NativeOnSelected(bool bBroadcast)
{
	Super::NativeOnSelected(bBroadcast);
}

void UModioCommonButtonBase::NativeOnDeselected(bool bBroadcast)
{
	Super::NativeOnDeselected(bBroadcast);
}

void UModioCommonButtonBase::NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnAddedToFocusPath(InFocusEvent);

	// Fix for Common UI not properly supporting hovering when switching between buttons using the keyboard (although seems to work fine on consoles)
	// To fix this, we create a new button style class programmatically and set it as the current style class

	if (ModioDefaultStyleClass)
	{
		UClass* NewFocusedStyleClass = [this]() {
			UClass* ButtonStyleClass = NewObject<UClass>();
			ButtonStyleClass->SetSuperStruct(ModioDefaultStyleClass);
			ButtonStyleClass->Bind();
			ButtonStyleClass->StaticLink(true);
			ButtonStyleClass->AssembleReferenceTokenStream();
			return ButtonStyleClass;
		}();

		if (NewFocusedStyleClass)
		{
			// To avoid GC
			ModioFocusedStyleClass = NewFocusedStyleClass;
			if (UCommonButtonStyle* FocusedButtonStyleObject = Cast<UCommonButtonStyle>(ModioFocusedStyleClass->GetDefaultObject()))
			{
				FocusedButtonStyleObject->NormalBase = FocusedButtonStyleObject->NormalHovered;
				FocusedButtonStyleObject->NormalTextStyle = FocusedButtonStyleObject->NormalHoveredTextStyle;
				if (UModioCommonButtonStyle* FocusedModioButtonStyleObject = Cast<UModioCommonButtonStyle>(ModioFocusedStyleClass->GetDefaultObject()))
				{
					FocusedModioButtonStyleObject->NormalIconStyle = FocusedModioButtonStyleObject->FocusedIconStyle;
				}
				Super::SetStyle(FocusedButtonStyleObject->GetClass());
			}
		}
	}

	SynchronizeProperties();
	OnButtonBaseFocusReceived.Broadcast(this);
}

void UModioCommonButtonBase::NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnRemovedFromFocusPath(InFocusEvent);

	// Reset the style class to the default one when the button is no longer focused
	if (ModioDefaultStyleClass)
	{
		SetStyle(ModioDefaultStyleClass);
	}

	SynchronizeProperties();

	OnFocusLost().Broadcast();
	OnButtonBaseFocusLost.Broadcast(this);
}

void UModioCommonButtonBase::NativeOnHovered() 
{
	Super::NativeOnHovered();

	SynchronizeProperties();
}

void UModioCommonButtonBase::NativeOnUnhovered()
{
	Super::NativeOnUnhovered();

	// Reset the style class to the default one when the button is no longer hovered
	if (ModioDefaultStyleClass)
	{
		SetStyle(ModioDefaultStyleClass);
	}

	SynchronizeProperties();
}

void UModioCommonButtonBase::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (UModioCommonButtonStyle* StyleCDO = Cast<UModioCommonButtonStyle>(Style.GetDefaultObject()))
	{
		if (Icon)
		{
			Icon->SetStyle(IsHovered() ? StyleCDO->FocusedIconStyle : StyleCDO->NormalIconStyle);
		}
	}
}
