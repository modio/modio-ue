﻿/*
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
UCLASS(Config = "ModioCommonAuthParams", DefaultConfig, meta = (DisplayName = "Auth Params"))
class MODIOUI5_API UModioCommonAuthParamsSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UModioCommonAuthParamsSettings()
	{
		BackInputAction.RowName = "Back";
		BackInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());

		SubmitInputAction.RowName = "Primary";
		SubmitInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());

		CancelInputAction.RowName = "Secondary";
		CancelInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());
	}

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = CommonInputActionDataBase), Category = "Actions")
	FDataTableRowHandle BackInputAction;

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = CommonInputActionDataBase), Category = "Actions")
	FDataTableRowHandle SubmitInputAction;

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = CommonInputActionDataBase), Category = "Actions")
	FDataTableRowHandle CancelInputAction;

	// Begin UDeveloperSettings Interface
	virtual FName GetCategoryName() const override { return ModioCommonCategoryName; }
	// End UDeveloperSettings Interface
};

/**
 * Project Settings customization for ModioCommonEmailAuthCodeView
 */
UCLASS(Config = "ModioCommonEmailAuthCodeParams", DefaultConfig, meta = (DisplayName = "Email Auth Code Params"))
class MODIOUI5_API UModioCommonEmailAuthCodeParamsSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText TitleText = NSLOCTEXT("Modio", "EmailAuthTitle", "Email Authenticating");

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

	// Begin UDeveloperSettings Interface
	virtual FName GetCategoryName() const override { return ModioCommonCategoryName; }
	// End UDeveloperSettings Interface
};

/**
 * Project Settings customization for ModioCommonEmailAuthLoadingView
 */
UCLASS(Config = "ModioCommonEmailAuthLoadingParams", DefaultConfig, meta = (DisplayName = "Email Auth Loading Params"))
class MODIOUI5_API UModioCommonEmailAuthLoadingParamsSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText TitleText = NSLOCTEXT("Modio", "EmailAuthLoadingTitle", "Email Authenticating");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText DescriptionText = NSLOCTEXT("Modio", "EmailAuthLoadingDescription", "Waiting for response ...");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText CancelButtonText = NSLOCTEXT("Modio", "CancelButtonText", "Cancel");

	// Begin UDeveloperSettings Interface
	virtual FName GetCategoryName() const override { return ModioCommonCategoryName; }
	// End UDeveloperSettings Interface
};

/**
 * Project Settings customization for ModioCommonEmailAuthView
 */
UCLASS(Config = "ModioCommonEmailAuthParams", DefaultConfig, meta = (DisplayName = "Email Auth Params"))
class MODIOUI5_API UModioCommonEmailAuthParamsSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText TitleText = NSLOCTEXT("Modio", "EmailAuthTitle", "Email Authenticating");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText DescriptionText = NSLOCTEXT("Modio", "EmailAuthDescription", "Please verify your email to continue");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText ValidationText = NSLOCTEXT("Modio", "EmailValidation", "Please enter a valid email address.");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText EmailTooltipText = NSLOCTEXT("Modio", "EmailAuthEmailTooltip", "Email");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText BackButtonText = NSLOCTEXT("Modio", "BackButtonText", "Back");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText SubmitButtonText = NSLOCTEXT("Modio", "SubmitButtonText", "Submit");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText CancelButtonText = NSLOCTEXT("Modio", "CancelButtonText", "Cancel");

	// Begin UDeveloperSettings Interface
	virtual FName GetCategoryName() const override { return ModioCommonCategoryName; }
	// End UDeveloperSettings Interface
};

/**
 * Project Settings customization for ModioCommonTermsOfUseView
 */
UCLASS(Config = "ModioCommonTermsOfUseParams", DefaultConfig, meta = (DisplayName = "Terms Of Use Params"))
class MODIOUI5_API UModioCommonTermsOfUseParamsSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText TitleText = NSLOCTEXT("Modio", "TermsOfUseTitle", "Terms Of Use");

	// Begin UDeveloperSettings Interface
	virtual FName GetCategoryName() const override { return ModioCommonCategoryName; }
	// End UDeveloperSettings Interface
};
