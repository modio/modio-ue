/* 
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *  
 *  This file is part of the mod.io UE4 Plugin.
 *  
 *  Distributed under the MIT License. (See accompanying file LICENSE or 
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *   
 */

using UnrealBuildTool;
using System.IO;
using System.Collections.Generic;

public class MSDFSupportEditor : ModuleRules
{
	public MSDFSupportEditor(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PrivateDependencyModuleNames.Add("Slate");
		PrivateDependencyModuleNames.Add("SlateCore");
		PrivateDependencyModuleNames.Add("PropertyEditor");
		PrivateDependencyModuleNames.Add("MainFrame");
		PrivateDependencyModuleNames.Add("AssetRegistry");
		PrivateDependencyModuleNames.Add("TextureEditor");
		PrivateDependencyModuleNames.Add("MaterialEditor");
		PrivateDependencyModuleNames.Add("RenderCore");
		PrivateDependencyModuleNames.Add("RHI");
        
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "UnrealEd", "MSDFSupport"});
 		
		PrivateIncludePaths.AddRange(new string[] {Path.Combine(ModuleDirectory, "Private") });

		PublicIncludePaths.AddRange(new string[] {Path.Combine(ModuleDirectory, "Public") });
	}
}