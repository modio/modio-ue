/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "ModioEditorSettings.h"

#include "Objects/ModioOpenSettingsAction.h"
#include "Objects/ModioOpenWeblinkAction.h"

#define LOCTEXT_NAMESPACE "ModioEditorSettings"

UModioEditorSettings::UModioEditorSettings(const FObjectInitializer& Initializer) : Super(Initializer)
{
	GettingStartedWidget = TSoftObjectPtr<UEditorUtilityWidgetBlueprint>(
		FSoftObjectPath("/Modio/GettingStarted/OnboardingGuideWidget.OnboardingGuideWidget"));

	//Maybe move these to some other file/namespace just for neatness.
	GettingStartedEntries.Add(FModioGettingStartedEntry(
		true, LOCTEXT("DocumentationEntryName", "Documentation"),
		LOCTEXT("DocumentationEntryDescription", "Plugin's concepts, how to guides, and class documentation"),
		TSoftObjectPtr<UTexture2D>(FSoftObjectPath("/Modio/GettingStarted/T_documentation_D.T_documentation_D")),
		UModioOpenWeblinkAction::StaticClass(), "https://docs.mod.io/unreal"));
	GettingStartedEntries.Add(FModioGettingStartedEntry(
		true, LOCTEXT("SettingsEntryName", "Settings"),
		LOCTEXT("SettingsEntryDescription", "Required settings for the plugin to function correctly"),
		TSoftObjectPtr<UTexture2D>(FSoftObjectPath("/Modio/GettingStarted/T_tutorials_D.T_tutorials_D")),
		UModioOpenSettingsAction::StaticClass(), ""));
	GettingStartedEntries.Add(FModioGettingStartedEntry(
		true, LOCTEXT("GithubEntryName", "mod.io Github"),
		LOCTEXT("GithubEntryDescription", "Access source code for our C++ SDK, Unreal and Unity plugins"),
		TSoftObjectPtr<UTexture2D>(FSoftObjectPath("/Modio/GettingStarted/T_github_D.T_github_D")),
		UModioOpenWeblinkAction::StaticClass(), "https://github.com/modio"));
	GettingStartedEntries.Add(FModioGettingStartedEntry(
		true, LOCTEXT("DiscordEntryName", "mod.io Discord"),
		LOCTEXT("DiscordEntryDescription", "Join our Discord to ask questions or talk to other developers adding UGC to their titles"),
		TSoftObjectPtr<UTexture2D>(FSoftObjectPath("/Modio/GettingStarted/T_discord_D.T_discord_D")),
		UModioOpenWeblinkAction::StaticClass(), "https://discord.mod.io"));
	GettingStartedEntries.Add(FModioGettingStartedEntry(
		true, LOCTEXT("RestApiEntryName", "REST API Documentation"),
		LOCTEXT("RestApiEntryDescription", "Documentation for our low-level REST API "),
		TSoftObjectPtr<UTexture2D>(FSoftObjectPath("/Modio/GettingStarted/T_api_D.T_api_D")),
		UModioOpenWeblinkAction::StaticClass(), "https://docs.mod.io/restapiref/"));
}

#undef LOCTEXT_NAMESPACE
