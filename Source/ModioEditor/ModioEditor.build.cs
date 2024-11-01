/* 
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *  
 *  This file is part of the mod.io UE Plugin.
 *  
 *  Distributed under the MIT License. (See accompanying file LICENSE or 
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *   
 */

using UnrealBuildTool;
using System.IO;

public class ModioEditor : ModuleRules
{
	public ModioEditor(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "UnrealEd", "Blutility", "LevelEditor", "UMGEditor", "UMG", "MainFrame", "Settings"});
 
        PublicIncludePaths.AddRange(new string[] { Path.Combine(ModuleDirectory, "Public") });
        PrivateIncludePaths.AddRange(new string[] { Path.Combine(ModuleDirectory, "Private") });

		PrivateDependencyModuleNames.AddRange(new string[] { "Projects", "InputCore", "ToolMenus", "Slate", "SlateCore", "Modio", "HTTP", "SharedSettingsWidgets"});
	}
}