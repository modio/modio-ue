/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/CommonComponents/ModioErrorRetryWidget.h"

TSharedRef<SWidget> UModioErrorRetryWidget::RebuildWidget()
{
	return Super::RebuildWidget();
}

void UModioErrorRetryWidget::SynchronizeProperties()
{
	if (ErrorText)
	{
		ErrorText->SetText(FText::FromString(ErrorTextValue));
		ErrorText->SynchronizeProperties();
	}

	if (RetryButton)
	{
		RetryButton->OnClicked.AddUniqueDynamic(this, &UModioErrorRetryWidget::OnRetryClicked);
	}

	Super::SynchronizeProperties();
}

void UModioErrorRetryWidget::OnRetryClicked()
{
	OnRetryRequestedDelegate.ExecuteIfBound();
}
