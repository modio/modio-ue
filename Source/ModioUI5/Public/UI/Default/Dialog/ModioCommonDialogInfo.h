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

/**
 * Enum of the different types of buttons that can be displayed in the dialog
 */
UENUM(BlueprintType, Meta = (BitFlags), Category = "Mod.io Common UI")
enum class EModioCommonDialogButtonType : uint8
{
	None		= 0 UMETA(Hidden),
	Back		= 1 << 0,
	Cancel		= 1 << 1,
	Confirm		= 1 << 2
};
ENUM_CLASS_FLAGS(EModioCommonDialogButtonType);

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

	/**
	 * Bitmask of the buttons to display in the dialog
	 */
	UPROPERTY(EditAnywhere, meta = (Bitmask, BitmaskEnum = "EModioCommonDialogButtonType"), Category = "Dialog")
	uint8 ButtonsToDisplay = 0;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogButtonClicked, EModioCommonDialogButtonType, ClickedButtonType);
	/**
	 * Dynamic delegate that is called when a button is clicked in the dialog
	 */
	UPROPERTY(BlueprintAssignable, Category = "Dialog")
	FOnDialogButtonClicked OnDialogButtonClicked;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnDialogButtonClickedFast, EModioCommonDialogButtonType);
	/**
	 * Static delegate that is called when a button is clicked in the dialog
	 */
	FOnDialogButtonClickedFast OnDialogButtonClickedFast;

	/**
	 * Stored property of the object that owns the dialog
	 */
	UPROPERTY(BlueprintReadWrite, Category = "Dialog")
	TObjectPtr<UObject> Owner;

	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI")
	void HandleDialogButtonClicked(EModioCommonDialogButtonType ClickedButtonType);
};