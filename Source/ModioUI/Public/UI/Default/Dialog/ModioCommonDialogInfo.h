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
#include "Misc/EnumClassFlags.h"
#include "Types/ModioModInfo.h"
#include "Types/ModioErrorCode.h"
#include "Kismet/BlueprintFunctionLibrary.h"
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
	Confirm		= 1 << 2,
	Ok			= 1 << 3,
	ModDetails	= 1 << 4
};
ENUM_CLASS_FLAGS(EModioCommonDialogButtonType);

/**
 * Data object that stores the information needed to display a dialog
 */
UCLASS(Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonDialogInfo : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * The title text of the dialog
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (MultiLine = true), Category = "Dialog")
	FText TitleText;

	/**
	 * The description text of the dialog
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (MultiLine = true), Category = "Dialog")
	FText DialogText;

	/**
	 * Bitmask of the buttons to display in the dialog
	 */
	UPROPERTY(EditAnywhere, meta = (Bitmask, BitmaskEnum = "/Script/ModioUI.EModioCommonDialogButtonType"), Category = "Dialog")
	uint8 ButtonsToDisplay = 0;

	/**
	 * Adds a button to the dialog
	 * @param ButtonType The type of button to add
	 */
	void AddButton(EModioCommonDialogButtonType ButtonType)
	{
		ButtonsToDisplay |= static_cast<uint8>(ButtonType);
	}

	/**
	 * Checks if the dialog has a button
	 * @param ButtonType The type of button to check for
	 * @return True if the dialog has the button, false otherwise
	 */
	bool HasButton(EModioCommonDialogButtonType ButtonType) const
	{
		return EnumHasAnyFlags(static_cast<EModioCommonDialogButtonType>(ButtonsToDisplay), ButtonType);
	}

	/**
	 * Removes a button from the dialog
	 * @param ButtonType The type of button to remove
	 */
	void RemoveButton(EModioCommonDialogButtonType ButtonType)
	{
		ButtonsToDisplay &= ~static_cast<uint8>(ButtonType);
	}

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

	/**
	 * Handles a button being clicked in the dialog
	 * @param ClickedButtonType The type of button that was clicked
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI")
	void HandleDialogButtonClicked(EModioCommonDialogButtonType ClickedButtonType);
};

/**
 * Class factory to create dialog parameters with different parameters
 */
UCLASS(NotBlueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonDialogLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Creates a dialog with the specified title and description
	 * @param TitleText The title text of the dialog
	 * @param DialogText The description text of the dialog
	 * @return The created dialog
	 */
	UFUNCTION(BlueprintCallable, Category = "Modio Common UI|Dialog")
	static UModioCommonDialogInfo* CreateManualDialogInfo(const FText& TitleText, const FText& DialogText);

	/**
	 * Creates a dialog with the specified error code and title
	 * @param ErrorCode The error code to display in the dialog
	 * @param TitleText The title text of the dialog
	 * @return The created dialog
	 */
	UFUNCTION(BlueprintCallable, Category = "Modio Common UI|Dialog")
	static UModioCommonDialogInfo* CreateErrorDialogInfo(const FModioErrorCode& ErrorCode, const FText& TitleText);

	/**
	 * Creates confirmation dialog for uninstalling a mod
	 * @param ModInfo The mod info to display in the dialog
	 * @return The created dialog
	 */
	UFUNCTION(BlueprintCallable, Category = "Modio Common UI|Dialog")
	static UModioCommonDialogInfo* CreateConfirmUninstallDialogInfo(const FModioModInfo& ModInfo);

	/**
	 * Creates a dialog for uninstalling a mod
	 * @param ModInfo 
	 * @return The created dialog
	 */
	UFUNCTION(BlueprintCallable, Category = "Modio Common UI|Dialog")
	static UModioCommonDialogInfo* CreateUninstallDialogInfo(const FModioModInfo& ModInfo);
};