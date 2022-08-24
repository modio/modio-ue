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
#include "Engine/Engine.h"
#include "Logging/LogVerbosity.h"
#include "UI/Commands/ModioCommonUICommands.h"

#define LOCTEXT_NAMESPACE "FModioUIModule"

void FModioUIModule::StartupModule()
{
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
	FModioCommonUICommands::Register();
}

void FModioUIModule::ShutdownModule()
{
	FModioCommonUICommands::Unregister();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FModioUIModule, ModioUI)
