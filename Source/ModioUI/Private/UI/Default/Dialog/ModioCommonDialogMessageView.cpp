/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Default/Dialog/ModioCommonDialogMessageView.h"
#include "UI/Foundation/Components/Text/TextBlock/ModioCommonTextBlock.h"
#include "UI/Foundation/Components/Button/ModioCommonButtonBase.h"
#include "UI/Foundation/Components/Text/EditableTextBox/ModioCommonEditableTextBox.h"
#include "Core/ModioModInfoUI.h"
#include "Libraries/ModioSDKLibrary.h"
#include "UI/Default/Dialog/ModioCommonDialogInfo.h"

void UModioCommonDialogMessageView::SynchronizeProperties() 
{
	Super::SynchronizeProperties();
}

void UModioCommonDialogMessageView::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	TMap<EModioCommonDialogButtonType, UModioCommonButtonBase*> AllButtons = GetButtons();
	for (auto& ButtonPair : AllButtons)
	{
		if (UModioCommonButtonBase* Button = ButtonPair.Value)
		{
			SetButtonEnabledState(Button, false);
			Button->OnClicked().AddWeakLambda(this, [this, ButtonType = ButtonPair.Key]() {
				if (UModioCommonDialogInfo* DialogInfo = Cast<UModioCommonDialogInfo>(DataSource))
				{
					DialogInfo->HandleDialogButtonClicked(ButtonType);
				}
			});
		}
	}
}

void UModioCommonDialogMessageView::NativeOnSetDataSource()
{
	Super::NativeOnSetDataSource();
	
	if (UModioCommonDialogInfo* DialogInfo = Cast<UModioCommonDialogInfo>(DataSource))
	{
		if (TitleTextBlock)
		{
			TitleTextBlock->SetText(DialogInfo->TitleText);
		}
		if (DescriptionTextBlock) 
		{
			DescriptionTextBlock->SetText(DialogInfo->DialogText);
		}

		TMap<EModioCommonDialogButtonType, UModioCommonButtonBase*> AllButtons = GetButtons();
		for (auto& ButtonPair : AllButtons)
		{
			SetButtonEnabledState(ButtonPair.Value, DialogInfo->HasButton(ButtonPair.Key));
		}
	}

	FocusOnDesiredWidget();
}

UWidget* UModioCommonDialogMessageView::NativeGetDesiredFocusTarget() const
{
	if (UWidget* WidgetToFocus = BP_GetDesiredFocusTarget())
	{
		return WidgetToFocus;
	}
	if (UModioCommonDialogInfo* DialogInfo = Cast<UModioCommonDialogInfo>(DataSource))
	{
		TMap<EModioCommonDialogButtonType, UModioCommonButtonBase*> AllButtons = GetButtons();
		for (auto& ButtonPair : AllButtons)
		{
			if (DialogInfo->HasButton(ButtonPair.Key))
			{
				return ButtonPair.Value;
			}
		}
	}
	return nullptr;
}

TMap<EModioCommonDialogButtonType, UModioCommonButtonBase*> UModioCommonDialogMessageView::GetButtons_Implementation() const
{
	TMap<EModioCommonDialogButtonType, UModioCommonButtonBase*> Buttons;
	if (BackButton)
	{
		Buttons.Add(EModioCommonDialogButtonType::Back, BackButton);
	}
	if (CancelButton)
	{
		Buttons.Add(EModioCommonDialogButtonType::Cancel, CancelButton);
	}
	if (ConfirmButton)
	{
		Buttons.Add(EModioCommonDialogButtonType::Confirm, ConfirmButton);
	}
	if (OkButton)
	{
		Buttons.Add(EModioCommonDialogButtonType::Ok, OkButton);
	}
	if (ModDetailsButton)
	{
		Buttons.Add(EModioCommonDialogButtonType::ModDetails, ModDetailsButton);
	}
	return Buttons;
}

void UModioCommonDialogMessageView::SetButtonEnabledState_Implementation(UModioCommonButtonBase* Button, bool bEnabled)
{
	if (Button)
	{
		Button->SetVisibility(bEnabled ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		Button->SetIsEnabled(bEnabled);
	}
}