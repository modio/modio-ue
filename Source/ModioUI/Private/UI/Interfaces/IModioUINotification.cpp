/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/Interfaces/IModioUINotification.h"

#include "Loc/BeginModioLocNamespace.h"
#include "Types/ModioErrorCode.h"
#include "UI/Interfaces/IModioModInfoUIDetails.h"

FModioNotificationParams UModioNotificationParamsLibrary::CreateErrorNotificationParams(FModioErrorCode StatusCode,
																						const FText& SuccessText,
																						const FText& ErrorText)
{
	FModioNotificationParams NewParams;
	NewParams.ErrorCode = StatusCode;
	NewParams.NamedTextFormats.Add("SuccessText", SuccessText);
	NewParams.NamedTextFormats.Add("ErrorText", ErrorText);
	return NewParams;
}

FModioNotificationParams UModioNotificationParamsLibrary::CreateRatingNotification(
	FModioErrorCode StatusCode, const TScriptInterface<IModioModInfoUIDetails>& ModInfo)
{
	FModioNotificationParams NewParams =
		CreateErrorNotificationParams(StatusCode, LOCTEXT("RatingSubmissionSucceeded", "Rating submitted"),
									  LOCTEXT("RatingSubmissionFailed", "Rating submission failed."));
	if (ModInfo.GetObject())
	{
		NewParams.NamedTextFormats.Add("MessageText") =
			FText::FromString(ModInfo->Execute_GetFullModInfo(ModInfo.GetObject()).ProfileName);
	}
	return NewParams;
}

FModioNotificationParams UModioNotificationParamsLibrary::CreateSubscriptionNotification(FModioErrorCode StatusCode)
{
	FModioNotificationParams NewParams =
		CreateErrorNotificationParams(StatusCode, LOCTEXT("SubscriptionSuccess", "Subscription Added!"),
									  LOCTEXT("SubscriptionFailed", "Subscription Failed"));
	return NewParams;
}

FModioNotificationParams UModioNotificationParamsLibrary::CreateUninstallNotification(FModioErrorCode StatusCode)
{
	FModioNotificationParams NewParams =
		CreateErrorNotificationParams(StatusCode, LOCTEXT("UninstallSuccess", "Mod Uninstalled!"),
									  LOCTEXT("SubscriptionFailed", "Subscription Failed"));
	return NewParams;
}

FModioNotificationParams UModioNotificationParamsLibrary::CreateInstallationNotification(FModioErrorCode StatusCode)
{
	FModioNotificationParams NewParams =
		CreateErrorNotificationParams(StatusCode, LOCTEXT("InstallationSuccess", "Mod installed"),
									  LOCTEXT("InstallationFailed", "Installation failed"));
	return NewParams;
}

#include "Loc/EndModioLocNamespace.h"