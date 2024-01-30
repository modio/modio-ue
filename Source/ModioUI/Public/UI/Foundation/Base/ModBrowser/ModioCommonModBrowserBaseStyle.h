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
#include "UObject/Object.h"
#include "Templates/SubclassOf.h"
#include "ModioCommonModBrowserBaseStyle.generated.h"

class UModioCommonModDetailsViewBase;
class UModioCommonAuthViewBase;
class UModioCommonReportViewBase;
class UModioCommonDialogViewBase;

/**
 * The style of the Mod Browser Base within the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonModBrowserBaseStyle : public UObject
{
	GENERATED_BODY()

public:
	/** Mod Details View class */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Mod Details")
	TSubclassOf<UModioCommonModDetailsViewBase> ModDetailsClass;

	/** User Auth View class */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Auth")
	TSubclassOf<UModioCommonAuthViewBase> UserAuthClass;

	/** Report View class */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Report")
	TSubclassOf<UModioCommonReportViewBase> ReportClass;

	/** Dialog View class */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Report")
	TSubclassOf<UModioCommonDialogViewBase> DialogClass;
};
