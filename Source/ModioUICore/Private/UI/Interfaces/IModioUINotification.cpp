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

FModioNotificationParams UModioNotificationParamsLibrary::CreateNotificationParams(FModioErrorCode StatusCode, const FText& TitleText, const FText& SuccessText, const FText& ErrorText)
{
	FModioNotificationParams NewParams;
	NewParams.ErrorCode = StatusCode;
	NewParams.NamedTextFormats.Add("TitleText", TitleText);
	NewParams.NamedTextFormats.Add("SuccessText", SuccessText);
	NewParams.NamedTextFormats.Add("ErrorText", ErrorText);
	return NewParams;
}

FModioNotificationParams UModioNotificationParamsLibrary::CreateRatingNotification(FModioErrorCode StatusCode, const TScriptInterface<IModioModInfoUIDetails>& ModInfo)
{
	FModioNotificationParams NewParams;
	if (ModInfo.GetObject())
	{
		const FModioModInfo FullModInfo = ModInfo->Execute_GetFullModInfo(ModInfo.GetObject());
		NewParams = CreateNotificationParams(StatusCode, FText::FromString(FullModInfo.ProfileName), LOCTEXT("RatingSubmissionSucceeded", "Rating submitted"), LOCTEXT("RatingSubmissionFailed", "Rating submission failed."));
	}
	return NewParams;
}

FModioNotificationParams UModioNotificationParamsLibrary::CreateSubscriptionNotification(FModioErrorCode StatusCode, const TScriptInterface<IModioModInfoUIDetails>& ModInfo)
{
	FModioNotificationParams NewParams;
	if (ModInfo.GetObject())
	{
		const FModioModInfo FullModInfo = ModInfo->Execute_GetFullModInfo(ModInfo.GetObject());
		NewParams = CreateNotificationParams(StatusCode, FullModInfo.ProfileName.IsEmpty() ? LOCTEXT("ErrorNotificationTitle", "Error") : FText::FromString(FullModInfo.ProfileName), LOCTEXT("SubscriptionSuccess", "Subscription Added!"), LOCTEXT("SubscriptionFailed", "Subscription Failed"));
	}
	return NewParams;
}

FModioNotificationParams UModioNotificationParamsLibrary::CreateUnsubscriptionNotification(FModioErrorCode StatusCode, const TScriptInterface<IModioModInfoUIDetails>& ModInfo)
{
	FModioNotificationParams NewParams;
	if (ModInfo.GetObject())
	{
		const FModioModInfo FullModInfo = ModInfo->Execute_GetFullModInfo(ModInfo.GetObject());
		NewParams = CreateNotificationParams(StatusCode, FText::FromString(FullModInfo.ProfileName), LOCTEXT("UnsubscriptionSuccess", "Subscription Removed!"), LOCTEXT("UnsubscriptionFailed", "Unsubscription Failed"));
	}
	return NewParams;
}

FModioNotificationParams UModioNotificationParamsLibrary::CreateUninstallNotification(FModioErrorCode StatusCode, const TScriptInterface<IModioModInfoUIDetails>& ModInfo)
{
	FModioNotificationParams NewParams;
	if (ModInfo.GetObject())
	{
		const FModioModInfo FullModInfo = ModInfo->Execute_GetFullModInfo(ModInfo.GetObject());
		NewParams = CreateNotificationParams(StatusCode, FText::FromString(FullModInfo.ProfileName), LOCTEXT("UninstallSuccess", "Mod Uninstalled!"), LOCTEXT("UninstallFailed", "Uninstall Failed"));
	}
	return NewParams;
}

FModioNotificationParams UModioNotificationParamsLibrary::CreateInstallationNotification(FModioErrorCode StatusCode, const TScriptInterface<IModioModInfoUIDetails>& ModInfo)
{
	FModioNotificationParams NewParams;
	if (ModInfo.GetObject())
	{
		const FModioModInfo FullModInfo = ModInfo->Execute_GetFullModInfo(ModInfo.GetObject());
		NewParams = CreateNotificationParams(StatusCode, FText::FromString(FullModInfo.ProfileName), LOCTEXT("InstallationSuccess", "Mod installed"), LOCTEXT("InstallationFailed", "Installation failed"));
	}
	return NewParams;
}

FModioNotificationParams UModioNotificationParamsLibrary::CreatePrioritizeTransferForModNotification(FModioErrorCode StatusCode, const TScriptInterface<IModioModInfoUIDetails>& ModInfo)
{
	FModioNotificationParams NewParams;
	if (ModInfo.GetObject())
	{
		const FModioModInfo FullModInfo = ModInfo->Execute_GetFullModInfo(ModInfo.GetObject());
		NewParams = CreateNotificationParams(StatusCode, FText::FromString(FullModInfo.ProfileName), LOCTEXT("PrioritizeTransferSuccess", "Prioritized for download"), LOCTEXT("PrioritizeTransferFailed", "Failed to prioritize for download"));
	}
	return NewParams;
}

#include "Loc/EndModioLocNamespace.h"