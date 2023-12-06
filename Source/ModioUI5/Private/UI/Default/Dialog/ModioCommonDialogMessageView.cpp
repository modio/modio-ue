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

	if (BackButton)
	{
		SetButtonEnabledState(BackButton, false);
		BackButton->OnClicked().AddWeakLambda(this, [this]() {
			if (UModioCommonDialogInfo* DialogInfo = Cast<UModioCommonDialogInfo>(DataSource))
			{
				DialogInfo->HandleDialogButtonClicked(EModioCommonDialogButtonType::Back);
			}
		});
	}

	if (CancelButton)
	{
		SetButtonEnabledState(CancelButton, false);
		CancelButton->OnClicked().AddWeakLambda(this, [this]() {
			if (UModioCommonDialogInfo* DialogInfo = Cast<UModioCommonDialogInfo>(DataSource))
			{
				DialogInfo->HandleDialogButtonClicked(EModioCommonDialogButtonType::Cancel);
			}
		});
	}

	if (ConfirmButton)
	{
		SetButtonEnabledState(ConfirmButton, false);
		ConfirmButton->OnClicked().AddWeakLambda(this, [this]() {
			if (UModioCommonDialogInfo* DialogInfo = Cast<UModioCommonDialogInfo>(DataSource))
			{
				DialogInfo->HandleDialogButtonClicked(EModioCommonDialogButtonType::Confirm);
			}
		});
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

		TArray<UModioCommonButtonBase*> ButtonsToDisplay = GetButtonsToDisplay();
		for (UModioCommonButtonBase* Button : ButtonsToDisplay)
		{
			SetButtonEnabledState(Button, true);
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
	TArray<UModioCommonButtonBase*> ButtonsToDisplay = GetButtonsToDisplay();
	for (UModioCommonButtonBase* Button : ButtonsToDisplay)
	{
		return Button;
	}
	return nullptr;
}

void UModioCommonDialogMessageView::SetButtonEnabledState_Implementation(UModioCommonButtonBase* Button, bool bEnabled)
{
	if (Button)
	{
		Button->SetVisibility(bEnabled ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		Button->SetIsEnabled(bEnabled);
	}
}

TArray<UModioCommonButtonBase*> UModioCommonDialogMessageView::GetButtonsToDisplay_Implementation() const
{
	TArray<UModioCommonButtonBase*> ButtonsToDisplay;
	if (UModioCommonDialogInfo* DialogInfo = Cast<UModioCommonDialogInfo>(DataSource))
	{
		if (BackButton && EnumHasAnyFlags(static_cast<EModioCommonDialogButtonType>(DialogInfo->ButtonsToDisplay), EModioCommonDialogButtonType::Back))
		{
			ButtonsToDisplay.Add(BackButton);
		}
		if (CancelButton && EnumHasAnyFlags(static_cast<EModioCommonDialogButtonType>(DialogInfo->ButtonsToDisplay), EModioCommonDialogButtonType::Cancel))
		{
			ButtonsToDisplay.Add(CancelButton);
		}
		if (ConfirmButton && EnumHasAnyFlags(static_cast<EModioCommonDialogButtonType>(DialogInfo->ButtonsToDisplay), EModioCommonDialogButtonType::Confirm))
		{
			ButtonsToDisplay.Add(ConfirmButton);
		}
	}
	return ButtonsToDisplay;
}
