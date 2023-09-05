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

#include "ModioCommonDialogInfo.generated.h"

UCLASS()
class MODIOUI5_API UModioCommonDialogInfo : public UObject
{
	GENERATED_BODY()

public:

	/**
	 * Stored property of the title text to display
	 **/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (MultiLine = true), Category = "Dialog")
	FText TitleText;

	/**
	 * Stored property of the text to display in the dialog box
	 **/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (MultiLine = true), Category = "Dialog")
	FText DialogText;
};