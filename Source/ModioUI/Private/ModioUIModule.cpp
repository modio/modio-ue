/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ModioUIModule.h"
#include "Core/Input/ModioInputKeys.h"
#include "CoreMinimal.h"
#include "ModioUI4Subsystem.h"
#include "Engine/Engine.h"
#include "HAL/IConsoleManager.h"
#include "Logging/LogVerbosity.h"
#include "Misc/EngineVersionComparison.h"
#include "ModioUISubsystem.h"
#include "UI/Commands/ModioCommonUICommands.h"
#include "UObject/CoreRedirects.h"

#define LOCTEXT_NAMESPACE "FModioUIModule"

void FModioUIModule::StartupModule()
{
#if UE_VERSION_NEWER_THAN(5, 0, 0)
	// Redirect ESlateColorStylingMode::UseColor_Specified_Link to ESlateColorStylingMode::UseColor_Specified
	FCoreRedirectObjectName ColorStyling;
	ColorStyling.ObjectName = "ESlateColorStylingMode";
	FCoreRedirect ColorStylingRedirect(ECoreRedirectFlags::Type_Enum, ColorStyling, ColorStyling);
	TMap<FString, FString> ColorStylingValueChanges;
	ColorStylingValueChanges.Add("UseColor_Specified_Link", "UseColor_Specified");
	ColorStylingRedirect.ValueChanges = ColorStylingValueChanges;

	TArray<FCoreRedirect> RedirectList = {ColorStylingRedirect};
	FCoreRedirects::AddRedirectList(RedirectList, "FModioUIModule::StartupModule()");
#endif

	EKeys::AddKey(FKeyDetails(FModioInputKeys::Up, LOCTEXT("ModioInputKey_Up", "Up"),
							  FKeyDetails::EKeyFlags::NotActionBindableKey, NAME_ModioInputKeys));
	EKeys::AddKey(FKeyDetails(FModioInputKeys::Down, LOCTEXT("ModioInputKey_Down", "Down"),
							  FKeyDetails::EKeyFlags::NotActionBindableKey, NAME_ModioInputKeys));
	EKeys::AddKey(FKeyDetails(FModioInputKeys::Left, LOCTEXT("ModioInputKey_Left", "Left"),
							  FKeyDetails::EKeyFlags::NotActionBindableKey, NAME_ModioInputKeys));
	EKeys::AddKey(FKeyDetails(FModioInputKeys::Right, LOCTEXT("ModioInputKey_Right", "Right"),
							  FKeyDetails::EKeyFlags::NotActionBindableKey, NAME_ModioInputKeys));
	EKeys::AddKey(FKeyDetails(FModioInputKeys::Back, LOCTEXT("ModioInputKey_Back", "Back"),
							  FKeyDetails::EKeyFlags::NotActionBindableKey, NAME_ModioInputKeys));
	EKeys::AddKey(FKeyDetails(FModioInputKeys::Confirm, LOCTEXT("ModioInputKey_Confirm", "Confirm"),
							  FKeyDetails::EKeyFlags::NotActionBindableKey, NAME_ModioInputKeys));
	EKeys::AddKey(FKeyDetails(FModioInputKeys::RefineSearch, LOCTEXT("ModioInputKey_RefineSearch", "Refine Search"),
							  FKeyDetails::EKeyFlags::NotActionBindableKey, NAME_ModioInputKeys));
	EKeys::AddKey(FKeyDetails(FModioInputKeys::MoreOptions, LOCTEXT("ModioInputKey_MoreOptions", "More Options"),
							  FKeyDetails::EKeyFlags::NotActionBindableKey, NAME_ModioInputKeys));
	EKeys::AddKey(FKeyDetails(FModioInputKeys::Previous, LOCTEXT("ModioInputKey_Previous", "Previous"),
							  FKeyDetails::EKeyFlags::NotActionBindableKey, NAME_ModioInputKeys));
	EKeys::AddKey(FKeyDetails(FModioInputKeys::Next, LOCTEXT("ModioInputKey_Next", "Next"),
							  FKeyDetails::EKeyFlags::NotActionBindableKey, NAME_ModioInputKeys));
	EKeys::AddKey(FKeyDetails(FModioInputKeys::Subscribe, LOCTEXT("ModioInputKey_Subscribe", "Subscribe"),
							  FKeyDetails::EKeyFlags::NotActionBindableKey, NAME_ModioInputKeys));
	EKeys::AddKey(FKeyDetails(FModioInputKeys::DownloadQueue, LOCTEXT("ModioInputKey_DownloadQueue", "DownloadQueue"),
							  FKeyDetails::EKeyFlags::NotActionBindableKey, NAME_ModioInputKeys));
	EKeys::AddKey(FKeyDetails(FModioInputKeys::EnableDisableMod, LOCTEXT("ModioInputKey_EnableDisableMod", "EnableDisableMod"),
							  FKeyDetails::EKeyFlags::NotActionBindableKey, NAME_ModioInputKeys));
	EKeys::AddKey(FKeyDetails(FModioInputKeys::Collapse, LOCTEXT("ModioInputKey_Collapse", "Collapse"),
							  FKeyDetails::EKeyFlags::NotActionBindableKey, NAME_ModioInputKeys));
	EKeys::AddKey(FKeyDetails(FModioInputKeys::LogOut, LOCTEXT("ModioInputKey_LogOut", "LogOut"),
							  FKeyDetails::EKeyFlags::NotActionBindableKey, NAME_ModioInputKeys));
	EKeys::AddKey(FKeyDetails(FModioInputKeys::RateUp, LOCTEXT("ModioInputKey_RateUp", "RateUp"),
							  FKeyDetails::EKeyFlags::NotActionBindableKey, NAME_ModioInputKeys));
	EKeys::AddKey(FKeyDetails(FModioInputKeys::RateDown, LOCTEXT("ModioInputKey_RateDown", "RateDown"),
							  FKeyDetails::EKeyFlags::NotActionBindableKey, NAME_ModioInputKeys));
	EKeys::AddKey(FKeyDetails(FModioInputKeys::Report, LOCTEXT("ModioInputKey_Report", "Report"),
							  FKeyDetails::EKeyFlags::NotActionBindableKey, NAME_ModioInputKeys));
	FModioCommonUICommands::Register();

	ConsoleCommands.Add(IConsoleManager::Get().RegisterConsoleCommand(
		TEXT("InputGlyphPS4"), TEXT("Preview Input Glyphs for PS4"), FConsoleCommandDelegate::CreateLambda([]() {
			if (UModioUI4Subsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUI4Subsystem>()) {
				Subsystem->SetControllerOverrideType(EModioUIInputMode::Playstation);
			}
		})));
	ConsoleCommands.Add(IConsoleManager::Get().RegisterConsoleCommand(
		TEXT("InputGlyphXBox"), TEXT("Preview Input Glyphs for XBox"), FConsoleCommandDelegate::CreateLambda([]() {
			if (UModioUI4Subsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUI4Subsystem>())
			{
				Subsystem->SetControllerOverrideType(EModioUIInputMode::XBox);
			}
		})));
	ConsoleCommands.Add(IConsoleManager::Get().RegisterConsoleCommand(
		TEXT("InputGlyphSwitch"), TEXT("Preview Input Glyphs for Switch"), FConsoleCommandDelegate::CreateLambda([]() {
			if (UModioUI4Subsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUI4Subsystem>())
			{
				Subsystem->SetControllerOverrideType(EModioUIInputMode::NintendoSwitch);
			}
		})));
	ConsoleCommands.Add(IConsoleManager::Get().RegisterConsoleCommand(
		TEXT("InputGlyphClear"), TEXT("Clear Preview Input Glyph Override"), FConsoleCommandDelegate::CreateLambda([]() {
			if (UModioUI4Subsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUI4Subsystem>())
			{
				Subsystem->ClearControllerOverride();
			}
		})));
}

void FModioUIModule::ShutdownModule()
{
	FModioCommonUICommands::Unregister();

	for (IConsoleCommand* Command : ConsoleCommands) {
		if (Command) {
			IConsoleManager::Get().UnregisterConsoleObject(Command);
		}
	}
	ConsoleCommands.Empty();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FModioUIModule, ModioUI)
