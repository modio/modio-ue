/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Default/ErrorWithRetry/ModioCommonErrorWithRetryWidget.h"
#include "UI/Default/ErrorWithRetry/ModioCommonErrorWithRetryWidgetStyle.h"
#include "UI/Foundation/Components/Button/ModioCommonButtonBase.h"
#include "UI/Foundation/Components/Image/ModioCommonImage.h"
#include "UI/Foundation/Components/Text/TextBlock/ModioCommonTextBlock.h"
#include "UI/Settings/ModioCommonUISettings.h"

UModioCommonErrorWithRetryWidget::UModioCommonErrorWithRetryWidget()
{
	bAutoBindInputAction = false;
}

void UModioCommonErrorWithRetryWidget::SetStyle(TSubclassOf<UModioCommonErrorWithRetryWidgetStyle> InStyle)
{
	if (InStyle && InStyle != ModioStyle)
	{
		ModioStyle = InStyle;
		SynchronizeProperties();
	}
}

void UModioCommonErrorWithRetryWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (UModioCommonErrorWithRetryWidgetStyle* StyleCDO = ModioStyle.GetDefaultObject())
	{
		if (IconImage)
		{
			IconImage->SetStyle(StyleCDO->IconImageStyle);
		}

		if (ErrorTitleTextBlock)
		{
			ErrorTitleTextBlock->SetStyle(StyleCDO->ErrorTitleTextBlockStyle);
		}

		if (ErrorDescriptionTextBlock)
		{
			ErrorDescriptionTextBlock->SetStyle(StyleCDO->ErrorDescriptionTextBlockStyle);
		}

		if (RetryButton)
		{
			RetryButton->SetStyle(StyleCDO->RetryButtonStyle);
		}
	}

	if (const UModioCommonUISettings* UISettings = GetDefault<UModioCommonUISettings>())
	{
		if (RetryButton)
		{
			RetryButton->SetLabel(UISettings->ErrorWithRetryParams.RetryButtonTextLabel);
		}
	}
}

void UModioCommonErrorWithRetryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (const UModioCommonUISettings* UISettings = GetDefault<UModioCommonUISettings>())
	{
		if (RetryButton)
		{
			ListenForInputAction(RetryButton, UISettings->ErrorWithRetryParams.RetryInputAction, UISettings->ErrorWithRetryParams.RetryButtonTextLabel, FOnModioCommonActivatableWidgetActionFiredFast::CreateWeakLambda(this, [this]() 
			{
				RetryClickedDelegate.Broadcast();
				RetryClickedDynamicDelegate.Broadcast();
				OnRetryRequestedDelegate.ExecuteIfBound();
			}));
		}
	}
}

void UModioCommonErrorWithRetryWidget::NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnAddedToFocusPath(InFocusEvent);
	UnbindInputActions();
	BindInputActions();
}

void UModioCommonErrorWithRetryWidget::NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnRemovedFromFocusPath(InFocusEvent);
	UnbindInputActions();
}

UWidget* UModioCommonErrorWithRetryWidget::NativeGetDesiredFocusTarget() const
{
	if (UWidget* WidgetToFocus = BP_GetDesiredFocusTarget())
	{
		return WidgetToFocus;
	}

	if (RetryButton)
	{
		return RetryButton;
	}
	
	return Super::NativeGetDesiredFocusTarget();
}

void UModioCommonErrorWithRetryWidget::NativeOnSetDataSource()
{
	Super::NativeOnSetDataSource();

	if (UModioCommonErrorWithRetryParamsUI* ErrorWithRetryParams = Cast<UModioCommonErrorWithRetryParamsUI>(DataSource))
	{
		if (ErrorTitleTextBlock)
		{
			ErrorTitleTextBlock->SetText(ErrorWithRetryParams->ErrorTitleTextLabel);
		}

		if (ErrorDescriptionTextBlock)
		{
			ErrorDescriptionTextBlock->SetText(ErrorWithRetryParams->ErrorDescriptionTextLabel);
		}
	}
}
