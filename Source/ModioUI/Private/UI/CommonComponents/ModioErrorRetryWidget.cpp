// Fill out your copyright notice in the Description page of Project Settings.

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
