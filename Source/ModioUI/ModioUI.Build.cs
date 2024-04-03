/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

using System.IO;
using UnrealBuildTool;

public class ModioUI : ModuleRules
{
	public ModioUI(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Projects"
		});

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"Slate",
			"SlateCore",
			"UMG",
			"Modio",
			"ModioUICore",
			"DeveloperSettings",
			"HTTP",
			"InputCore"
		});

		if (Target.Version.MajorVersion >= 5 && Target.Version.MinorVersion >= 0)
		{
			PublicDependencyModuleNames.AddRange(new string[]
			{
				"CommonUI",
				"CommonInput",
				"GameplayTags"
			});
		}

		PublicIncludePaths.AddRange(new string[]
		{
			Path.Combine(ModuleDirectory, "Public")
		});

		PrivateIncludePaths.AddRange(new string[]
		{
			Path.Combine(ModuleDirectory, "Private")
		});
    }
}