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
#include "UI/Settings/Params/ModioCommonDialogParams.h"
#include "UI/Foundation/Components/Text/TextBlock/ModioCommonTextBlock.h"
#include "UI/Foundation/Components/Button/ModioCommonButtonBase.h"
#include "UI/Foundation/Components/Text/EditableTextBox/ModioCommonEditableTextBox.h"
#include "Core/ModioModInfoUI.h"
#include "Libraries/ModioSDKLibrary.h"
#include "UI/Default/Dialog/ModioCommonDialogInfo.h"

void UModioCommonDialogMessageView::SynchronizeProperties() 
{
	Super::SynchronizeProperties();

	if (const UModioCommonDialogMessageParamsSettings* Settings = GetDefault<UModioCommonDialogMessageParamsSettings>())
	{
		if (SubmitButton)
		{
			SubmitButton->SetLabel(Settings->SubmitButtonText);
		}
	}
}

void UModioCommonDialogMessageView::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	const UModioCommonDialogParamsSettings* DialogSettings = GetDefault<UModioCommonDialogParamsSettings>();
	const UModioCommonDialogMessageParamsSettings* DialogMessageSettings = GetDefault<UModioCommonDialogMessageParamsSettings>();
	if (!DialogSettings || !DialogMessageSettings)
	{
		return;
	}

	if (SubmitButton)
	{
		ListenForInputAction(SubmitButton, DialogSettings->SubmitInputAction, DialogMessageSettings->SubmitButtonText, [this]() 
		{
			if (OnSubmitClicked.IsBound()) OnSubmitClicked.Execute();
		});
	}
}

void UModioCommonDialogMessageView::NativeOnSetDataSource()
{
	Super::NativeOnSetDataSource();

	UModioCommonDialogInfo* DialogInfo = Cast<UModioCommonDialogInfo>(DataSource);
	if (DialogInfo)
	{
		if (TitleTextBlock)
		{
			TitleTextBlock->SetText(DialogInfo->TitleText);
		}
		if (DescriptionTextBlock) 
		{
			DescriptionTextBlock->SetText(DialogInfo->DialogText);
		}
	}
}

UWidget* UModioCommonDialogMessageView::NativeGetDesiredFocusTarget() const
{
	if (UWidget* WidgetToFocus = BP_GetDesiredFocusTarget())
	{
		return WidgetToFocus;
	}
	return SubmitButton.Get();
}