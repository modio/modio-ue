/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/DeveloperSettings.h"
#include "UI/Settings/ModioCommonDefines.h"
#include "ModioCommonAuthParams.generated.h"

/**
 * Project Settings customization for ModioCommonAuthView
 */
USTRUCT(BlueprintType, Category = "Mod.io Common UI")
struct MODIOUI_API FModioCommonAuthParamsSettings
{
	GENERATED_BODY()

	FModioCommonAuthParamsSettings()
	{
		BackInputAction.RowName = "Back";
		BackInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());

		SubmitInputAction.RowName = "Primary";
		SubmitInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());

		CancelInputAction.RowName = "Secondary";
		CancelInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());

		PrivacyInputAction.RowName = "LeftTabSecondary";
		PrivacyInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());

		TermsInputAction.RowName = "RightTabSecondary";
		TermsInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());
	}

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"), Category = "Actions")
	FDataTableRowHandle BackInputAction;

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"), Category = "Actions")
	FDataTableRowHandle SubmitInputAction;

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"), Category = "Actions")
	FDataTableRowHandle CancelInputAction;

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"), Category = "Actions")
	FDataTableRowHandle TermsInputAction;

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"), Category = "Actions")
	FDataTableRowHandle PrivacyInputAction;
};

/**
 * Project Settings customization for ModioCommonEmailAuthCodeView
 */
USTRUCT(BlueprintType, Category = "Mod.io Common UI")
struct MODIOUI_API FModioCommonEmailAuthCodeParamsSettings
{
	GENERATED_BODY()

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText TitleText = NSLOCTEXT("Modio", "EmailAuthTitle", "Email Authentication");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText DescriptionText = NSLOCTEXT("Modio", "EmailAuthDescription", "Once the verification email is received, enter the 5-digit code provided.");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText ValidationText = NSLOCTEXT("Modio", "CodeValidation", "Please enter the verification code that you have received.");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText BackButtonText = NSLOCTEXT("Modio", "BackButtonText", "Back");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText SubmitButtonText = NSLOCTEXT("Modio", "SubmitButtonText", "Submit");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText CancelButtonText = NSLOCTEXT("Modio", "CancelButtonText", "Cancel");
};

/**
 * Project Settings customization for ModioCommonEmailAuthLoadingView
 */
USTRUCT(BlueprintType, Category = "Mod.io Common UI")
struct MODIOUI_API FModioCommonEmailAuthLoadingParamsSettings
{
	GENERATED_BODY()

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText TitleText = NSLOCTEXT("Modio", "EmailAuthLoadingTitle", "Email Authentication");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText DescriptionText = NSLOCTEXT("Modio", "EmailAuthLoadingDescription", "Waiting for response...");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText CancelButtonText = NSLOCTEXT("Modio", "CancelButtonText", "Cancel");
};

/**
 * Project Settings customization for ModioCommonEmailAuthView
 */
USTRUCT(BlueprintType, Category = "Mod.io Common UI")
struct MODIOUI_API FModioCommonEmailAuthParamsSettings
{
	GENERATED_BODY()

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText TitleText = NSLOCTEXT("Modio", "EmailAuthTitle", "Email Authentication");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText DescriptionText = NSLOCTEXT("Modio", "EmailAuthDescription", "Please verify your email to continue");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText ValidationText = NSLOCTEXT("Modio", "EmailValidation", "Please enter a valid email address");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText EmailTooltipText = NSLOCTEXT("Modio", "EmailAuthEmailTooltip", "Email");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText BackButtonText = NSLOCTEXT("Modio", "BackButtonText", "Back");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText SubmitButtonText = NSLOCTEXT("Modio", "SubmitButtonText", "Submit");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText CancelButtonText = NSLOCTEXT("Modio", "CancelButtonText", "Cancel");
};

/**
 * Project Settings customization for ModioCommonTermsOfUseView
 */
USTRUCT(BlueprintType, Category = "Mod.io Common UI")
struct MODIOUI_API FModioCommonTermsOfUseParamsSettings
{
	GENERATED_BODY()

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText TitleText = NSLOCTEXT("Modio", "TermsOfUseTitle", "Terms Of Use");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText ContentText = NSLOCTEXT("Modio", "ContentText", "We use mod.io to support user-generated content in-game. By clicking \"I Agree\" you agree to the mod.io Terms of Use and a mod.io account will be created for you (using your display name, avatar and ID). Please see the mod.io Privacy Policy on how mod.io processes your personal data.");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText SubmitButtonText = NSLOCTEXT("Modio", "SubmitButtonText", "I Agree");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText CancelButtonText = NSLOCTEXT("Modio", "CancelButtonText", "No, Thanks");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText TermsButtonText = NSLOCTEXT("Modio", "TermsOfUseTitle", "Terms of Use");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText PrivacyButtonText = NSLOCTEXT("Modio", "TermsOfUseTitle", "Privacy Policy");
};
