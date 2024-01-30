/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Foundation/Components/Notification/ModioCommonNotificationWidget.h"
#include "UI/Foundation/Components/Text/RichTextBlock/ModioCommonRichTextBlock.h"
#include "Loc/BeginModioLocNamespace.h"
#include "UI/Foundation/Components/Image/ModioCommonImage.h"
#include "UI/Foundation/Components/Border/ModioCommonBorder.h"
#include "UI/Foundation/Components/Notification/ModioCommonNotificationWidgetStyle.h"

void UModioCommonNotificationWidget::SetStyle(TSubclassOf<UModioCommonNotificationWidgetStyle> InStyle)
{
	if (InStyle && InStyle != ModioStyle)
	{
		ModioStyle = InStyle;
		SynchronizeProperties();
	}
}

void UModioCommonNotificationWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	if (UModioCommonNotificationWidgetStyle* StyleCDO = ModioStyle.GetDefaultObject())
	{
		if (BackgroundBorder)
		{
			BackgroundBorder->SetStyle(StyleCDO->BackgroundBorderStyle);
		}

		if (StatusBackgroundBorder)
		{
			StatusBackgroundBorder->SetStyle(StyleCDO->StatusBackgroundBorderStyle);
		}

		if (StatusIndicatorImage)
		{
			StatusIndicatorImage->SetStyle(StyleCDO->StatusIndicatorImageStyle);
		}

		if (NotificationTitleTextBlock)
		{
			NotificationTitleTextBlock->SetStyle(StyleCDO->NotificationTitleTextStyle);
		}

		if (NotificationMessageTextBlock)
		{
			NotificationMessageTextBlock->SetStyle(StyleCDO->NotificationMessageTextStyle);
		}
	}
}

void UModioCommonNotificationWidget::NativeConfigure(const FModioNotificationParams& Params)
{
	Super::NativeConfigure(Params);
	bool bIsError = Params.ErrorCode;

#if WITH_EDITORONLY_DATA
	if (IsDesignTime())
	{
		bIsError = bPreviewAsError;
	}
#endif

	if (NotificationMessageTextBlock)
	{
		if (bIsError)
		{
			if (Params.NamedTextFormats.Contains(FName("ErrorText")))
			{
				NotificationMessageTextBlock->SetText(FText::Format(Params.NamedTextFormats["ErrorText"], Params.FormatArgs));
				NotificationMessageTextBlock->SetVisibility(ESlateVisibility::HitTestInvisible);
			}
			else
			{
#if WITH_EDITOR
				if (IsDesignTime())
				{
					NotificationMessageTextBlock->SetText(LOCTEXT("GenericErrorText", "Error message"));
				}
				else
#endif
				{
					NotificationMessageTextBlock->SetVisibility(ESlateVisibility::Collapsed);
				}
			}
		}
		else
		{
			if (Params.NamedTextFormats.Contains(FName("SuccessText")))
			{
				NotificationMessageTextBlock->SetText(FText::Format(Params.NamedTextFormats["SuccessText"], Params.FormatArgs));
			}
			else
			{
#if WITH_EDITOR
				if (IsDesignTime())
				{
					NotificationMessageTextBlock->SetText(LOCTEXT("GenericSuccessText", "Success message"));
				}
				else
#endif
				{
					NotificationMessageTextBlock->SetVisibility(ESlateVisibility::Collapsed);
				}
			}
		}
	}

	if (NotificationTitleTextBlock)
	{
		if (Params.NamedTextFormats.Contains(FName("TitleText")))
		{
			NotificationTitleTextBlock->SetText(FText::Format(Params.NamedTextFormats["TitleText"], Params.FormatArgs));
		}
		else
		{
			NotificationTitleTextBlock->SetText(LOCTEXT("GenericSuccessText", "Success!"));
		}
	}
}

void UModioCommonNotificationWidget::NativeConfigureManual(const FText& Title, const FText& Message, bool bIsError)
{
	Super::NativeConfigureManual(Title, Message, bIsError);
	if (NotificationTitleTextBlock)
	{
		NotificationTitleTextBlock->SetText(Title);
	}
	if (NotificationMessageTextBlock)
	{
		NotificationMessageTextBlock->SetText(Message);
		NotificationMessageTextBlock->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UModioCommonNotificationWidget::NativeOnDisplay()
{
	Super::NativeOnDisplay();
	bDisplayStarted = true;
	if (!FMath::IsNearlyZero(ExpiryDuration))
	{
		StartTime = FDateTime::Now();
	}
}

void UModioCommonNotificationWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (bDisplayStarted)
	{
		if ((FDateTime::Now() - StartTime).GetTotalSeconds() > ExpiryDuration)
		{
			ExpireDelegate.ExecuteIfBound(this);
		}
	}
}

#include "Loc/EndModioLocNamespace.h"