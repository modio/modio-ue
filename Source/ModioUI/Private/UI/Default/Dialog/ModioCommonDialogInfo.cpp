/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Default/Dialog/ModioCommonDialogInfo.h"

#include "ModioUI.h"
#include "ModioUISubsystem.h"
#include "UI/Foundation/Base/ModioCommonActivatableWidget.h"

void UModioCommonDialogInfo::HandleDialogButtonClicked(EModioCommonDialogButtonType ClickedButtonType)
{
	OnDialogButtonClicked.Broadcast(ClickedButtonType);
	OnDialogButtonClickedFast.Broadcast(ClickedButtonType);
}

UModioCommonDialogInfo* UModioCommonDialogLibrary::CreateManualDialogInfo(const FText& TitleText, const FText& DialogText)
{
	if (UModioCommonDialogInfo* DialogInfo = NewObject<UModioCommonDialogInfo>())
	{
		DialogInfo->TitleText = TitleText;
		DialogInfo->DialogText = DialogText;
		DialogInfo->AddButton(EModioCommonDialogButtonType::Ok);
		DialogInfo->OnDialogButtonClickedFast.AddWeakLambda(DialogInfo, [DialogInfo](EModioCommonDialogButtonType ButtonType) {
			if (UModioCommonActivatableWidget* DialogWidget = Cast<UModioCommonActivatableWidget>(DialogInfo->Owner.Get()))
			{
				DialogWidget->DeactivateWidget();
			}
		});
		return DialogInfo;
	}

	UE_LOG(ModioUI, Error, TEXT("Unable to create dialog: Unable to create dialog info"));
	return nullptr;
}

UModioCommonDialogInfo* UModioCommonDialogLibrary::CreateErrorDialogInfo(const FModioErrorCode& ErrorCode, const FText& TitleText)
{
	return CreateManualDialogInfo(TitleText, FText::FromString(ErrorCode.GetErrorMessage()));
}

UModioCommonDialogInfo* UModioCommonDialogLibrary::CreateConfirmUninstallDialogInfo(const FModioModInfo& ModInfo)
{
	if (UModioCommonDialogInfo* DialogInfo = NewObject<UModioCommonDialogInfo>())
	{
		DialogInfo->TitleText = NSLOCTEXT("Modio", "ForceUninstallTitle", "Are you sure?");
		DialogInfo->DialogText = FText::Format(NSLOCTEXT("Modio", "ForceUninstallText", "This will uninstall {0} for all users on this system"), FText::FromString(ModInfo.ProfileName));
		DialogInfo->AddButton(EModioCommonDialogButtonType::Confirm);
		DialogInfo->AddButton(EModioCommonDialogButtonType::Cancel);
		DialogInfo->OnDialogButtonClickedFast.AddWeakLambda(DialogInfo, [DialogInfo, ModInfo](EModioCommonDialogButtonType ButtonType) {
			if (ButtonType == EModioCommonDialogButtonType::Confirm)
			{
				if (UModioUISubsystem* ModioUISubsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
				{
					ModioUISubsystem->RequestUninstallForModID(ModInfo.ModId, FOnErrorOnlyDelegateFast::CreateWeakLambda(DialogInfo, [ModInfo](FModioErrorCode ErrorCode) {
						if (ErrorCode)
						{
							UE_LOG(ModioUI, Error, TEXT("Unable to force uninstall for mod '%s': %s"), *ModInfo.ProfileName, *ErrorCode.GetErrorMessage());
						}
						else
						{
							if (UModioUISubsystem* ModioUISubsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
							{
								if (UModioCommonDialogInfo* UninstallDialogInfo = UModioCommonDialogLibrary::CreateUninstallDialogInfo(ModInfo))
								{
									ModioUISubsystem->ShowDialog(UninstallDialogInfo);	
								}
							}
						}
					}));
				}
			}

			if (UModioCommonActivatableWidget* DialogWidget = Cast<UModioCommonActivatableWidget>(DialogInfo->Owner.Get()))
			{
				DialogWidget->DeactivateWidget();
			}
		});
		return DialogInfo;
	}

	UE_LOG(ModioUI, Error, TEXT("Unable to create confirm uninstall dialog for mod '%s': Unable to create dialog info"), *ModInfo.ProfileName);
	return nullptr;
}

UModioCommonDialogInfo* UModioCommonDialogLibrary::CreateUninstallDialogInfo(const FModioModInfo& ModInfo)
{
	if (UModioCommonDialogInfo* DialogInfo = NewObject<UModioCommonDialogInfo>())
	{
		DialogInfo->TitleText = FText::FromString(ModInfo.ProfileName);
		DialogInfo->DialogText = NSLOCTEXT("Modio", "UninstallSuccessText", "This mod has been successfully uninstalled.");
		DialogInfo->AddButton(EModioCommonDialogButtonType::Ok);
		DialogInfo->AddButton(EModioCommonDialogButtonType::ModDetails);
		DialogInfo->OnDialogButtonClickedFast.AddWeakLambda(DialogInfo, [DialogInfo, ModInfo](EModioCommonDialogButtonType ButtonType) {
			if (ButtonType == EModioCommonDialogButtonType::ModDetails)
			{
				UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
				if (!Subsystem)
				{
					UE_LOG(ModioUI, Error, TEXT("Unable to subscribe/unsubscribe for mod '%s': Modio UI Subsystem is invalid"), *ModInfo.ProfileName);
					return;
				}
				Subsystem->ShowDetailsForMod(ModInfo.ModId);
			}
			if (UModioCommonActivatableWidget* DialogWidget = Cast<UModioCommonActivatableWidget>(DialogInfo->Owner.Get()))
			{
				DialogWidget->DeactivateWidget();
			}
		});
		return DialogInfo;
	}

	UE_LOG(ModioUI, Error, TEXT("Unable to create uninstall dialog for mod '%s': Unable to create dialog info"), *ModInfo.ProfileName);
	return nullptr;
}

