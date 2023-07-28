/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#pragma once

#include "Core/Input/ModioInputKeys.h"
#include "Framework/Commands/Commands.h"
#include "Framework/Commands/UIAction.h"
#include "Framework/Commands/UICommandInfo.h"
#include "Internationalization/Internationalization.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Templates/SharedPointer.h"
#include "Textures/SlateIcon.h"
#include "UI/Commands/ModioUIMenuEntry.h"

#include "ModioCommonUICommands.generated.h"

class FModioCommonUICommands : public TCommands<FModioCommonUICommands>
{
public:
	TSharedPtr<FUICommandInfo> PreviousPage; // L-bumper
	TSharedPtr<FUICommandInfo> NextPage; // R-bumper

	TSharedPtr<FUICommandInfo> MoreOptions; // Y-button for More Options
	TSharedPtr<FUICommandInfo> Confirm; // A-button
	TSharedPtr<FUICommandInfo> RefineSearch; // right thumbstick button
	TSharedPtr<FUICommandInfo> DownloadQueue; // View button
	TSharedPtr<FUICommandInfo> Back; // B-button
	TSharedPtr<FUICommandInfo> Subscribe; // X-button
	TSharedPtr<FUICommandInfo> EnableDisableMod; // Left trigger
	TSharedPtr<FUICommandInfo> Collapse; // L-Trigger
	FModioCommonUICommands()
		: TCommands<FModioCommonUICommands>("ModioUI", FText::FromString("Modio UI"), NAME_None,
											FName("ModioUIStyleSet")) {};
	/// @brief Constructs a null FUIAction that does nothing. Useful for where you want to eat a command binding instead
	/// of passing it up the tree
	/// @return FUIAction instance that never executes
	static FUIAction GetNullAction();
	virtual void RegisterCommands() override;
};

UCLASS()
class MODIOUI_API UModioUICommandInfoLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "ModioCommonUICommands")
	static struct FModioUIMenuEntry PreviousPageCommand();
	UFUNCTION(BlueprintCallable, Category = "ModioCommonUICommands")
	static FModioUIMenuEntry NextPageCommand();
	UFUNCTION(BlueprintCallable, Category = "ModioCommonUICommands")
	static FModioUIMenuEntry MoreOptionsCommand();
	UFUNCTION(BlueprintCallable, Category = "ModioCommonUICommands")
	static FModioUIMenuEntry ConfirmCommand();
	UFUNCTION(BlueprintCallable, Category = "ModioCommonUICommands")
	static FModioUIMenuEntry RefineSearchCommand();
	UFUNCTION(BlueprintCallable, Category = "ModioCommonUICommands")
	static FModioUIMenuEntry BackCommand();
	UFUNCTION(BlueprintCallable, Category = "ModioCommonUICommands")
	static FModioUIMenuEntry DownloadQueueCommand();
	UFUNCTION(BlueprintCallable, Category = "ModioCommonUICommands")
	static FModioUIMenuEntry CollapseCommand();
};
