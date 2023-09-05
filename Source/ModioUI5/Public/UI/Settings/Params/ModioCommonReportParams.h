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
#include "ModioCommonReportParams.generated.h"

/**
 * Project Settings customization for ModioCommonReportView
 */
UCLASS(Config = "ModioCommonReportParams", DefaultConfig, meta = (DisplayName = "Report Params"))
class MODIOUI5_API UModioCommonReportParamsSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UModioCommonReportParamsSettings()
	{
		ProceedInputAction.RowName = "Primary";
		ProceedInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());

		BackInputAction.RowName = "Back";
		BackInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());

		SubmitInputAction.RowName = "Primary";
		SubmitInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());

		CancelInputAction.RowName = "Secondary";
		CancelInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());
	}

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = CommonInputActionDataBase), Category = "Actions")
	FDataTableRowHandle ProceedInputAction;

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
 * Project Settings customization for ModioCommonReportReasonView
 */
UCLASS(Config = "ModioCommonReportReasonParams", DefaultConfig, meta = (DisplayName = "Report Reason Params"))
class MODIOUI5_API UModioCommonReportReasonParamsSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText DescriptionText = NSLOCTEXT("Modio", "ReportReasonDescription", "Please select the reason(s) for reporting a mod.");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText ProceedButtonText = NSLOCTEXT("Modio", "ProceedButtonText", "Proceed");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText CancelButtonText = NSLOCTEXT("Modio", "CancelButtonText", "Cancel");

	// Begin UDeveloperSettings Interface
	virtual FName GetCategoryName() const override { return ModioCommonCategoryName; }
	// End UDeveloperSettings Interface
};

/**
 * Project Settings customization for ModioCommonReportEmailView
 */
UCLASS(Config = "ModioCommonReportEmailParams", DefaultConfig, meta = (DisplayName = "Report Email Params"))
class MODIOUI5_API UModioCommonReportEmailParamsSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText DescriptionText = NSLOCTEXT("Modio", "ReportEmailDescription", "Your email may be shared with moderators and the person that posted the allegedly infringing content you are reporting.");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText ValidationText = NSLOCTEXT("Modio", "EmailValidation", "Please enter a valid email address.");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText EmailTooltipText = NSLOCTEXT("Modio", "ReportEmailTooltip", "Email");

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
 * Project Settings customization for ModioCommonReportMessageView
 */
UCLASS(Config = "ModioCommonReportMessageParams", DefaultConfig, meta = (DisplayName = "Report Message Params"))
class MODIOUI5_API UModioCommonReportMessageParamsSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText DescriptionText = NSLOCTEXT("Modio", "ReportMessageDescription", "To help us process your report, please provide as much detail and evidence as possible.");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText ValidationText = NSLOCTEXT("Modio", "MessageValidation", "Max characters: 10,000");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	int32 MessageLength = 10000;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText MessageTooltipText = NSLOCTEXT("Modio", "ReportMessageTooltip", "Report Message");

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
 * Project Settings customization for ModioCommonReportSummaryView
 */
UCLASS(Config = "ModioCommonReportSummaryParams", DefaultConfig, meta = (DisplayName = "Report Summary Params"))
class MODIOUI5_API UModioCommonReportSummaryParamsSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText DescriptionText = NSLOCTEXT("Modio", "ReportSummaryDescription", "Report Summary");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText ReasonLabelText = NSLOCTEXT("Modio", "ReasonLabelText", "Reason:");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText EmailLabelText = NSLOCTEXT("Modio", "EmailLabelText", "Email:");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText DetailsLabelText = NSLOCTEXT("Modio", "DetailsLabelText", "Details:");

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