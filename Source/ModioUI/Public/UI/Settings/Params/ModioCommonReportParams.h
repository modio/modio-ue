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
USTRUCT(BlueprintType, Category = "Mod.io Common UI")
struct MODIOUI_API FModioCommonReportParamsSettings
{
	GENERATED_BODY()

public:
	virtual ~FModioCommonReportParamsSettings() = default;

	FModioCommonReportParamsSettings()
	{
		ProceedInputAction.RowName = "Secondary";
		ProceedInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());

		BackInputAction.RowName = "Back";
		BackInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());

		SubmitInputAction.RowName = "Secondary";
		SubmitInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());

		CancelInputAction.RowName = "Tertiary";
		CancelInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());
	}

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"), Category = "Actions")
	FDataTableRowHandle ProceedInputAction;

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"), Category = "Actions")
	FDataTableRowHandle BackInputAction;

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"), Category = "Actions")
	FDataTableRowHandle SubmitInputAction;

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"), Category = "Actions")
	FDataTableRowHandle CancelInputAction;
};

/**
 * Project Settings customization for ModioCommonReportReasonView
 */
USTRUCT(BlueprintType, Category = "Mod.io Common UI")
struct MODIOUI_API FModioCommonReportReasonParamsSettings
{
	GENERATED_BODY()

public:

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText DescriptionText = NSLOCTEXT("Modio", "ReportReasonDescription", "Please select a reason for reporting this mod");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText ProceedButtonText = NSLOCTEXT("Modio", "ProceedButtonText", "Proceed");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText CancelButtonText = NSLOCTEXT("Modio", "CancelButtonText", "Cancel");
};

/**
 * Project Settings customization for ModioCommonReportEmailView
 */
USTRUCT(BlueprintType, Category = "Mod.io Common UI")
struct MODIOUI_API FModioCommonReportEmailParamsSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText DescriptionText = NSLOCTEXT("Modio", "ReportEmailDescription", "Your email may be shared with moderators and the person that posted the allegedly infringing content");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText ValidationText = NSLOCTEXT("Modio", "EmailValidation", "Please enter a valid email address.");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText EmailTooltipText = NSLOCTEXT("Modio", "ReportEmailTooltip", "Email");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText BackButtonText = NSLOCTEXT("Modio", "BackButtonText", "Back");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText SubmitButtonText = NSLOCTEXT("Modio", "SubmitButtonText", "Next");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText CancelButtonText = NSLOCTEXT("Modio", "CancelButtonText", "Cancel");
};

/**
 * Project Settings customization for ModioCommonReportMessageView
 */
USTRUCT(BlueprintType, Category = "Mod.io Common UI")
struct MODIOUI_API FModioCommonReportMessageParamsSettings
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
	FText SubmitButtonText = NSLOCTEXT("Modio", "SubmitButtonText", "Next");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText CancelButtonText = NSLOCTEXT("Modio", "CancelButtonText", "Cancel");
};

/**
 * Project Settings customization for ModioCommonReportSummaryView
 */
USTRUCT(BlueprintType, Category = "Mod.io Common UI")
struct MODIOUI_API FModioCommonReportSummaryParamsSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText DescriptionText = NSLOCTEXT("Modio", "ReportSummaryDescription", "Summary");

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
};