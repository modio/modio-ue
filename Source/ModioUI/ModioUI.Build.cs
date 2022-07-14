/* 
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *  
 *  This file is part of the mod.io UE4 Plugin.
 *  
 *  Distributed under the MIT License. (See accompanying file LICENSE or 
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *   
 */

using System.IO;
using System.Collections.Generic;
using UnrealBuildTool;

public class ModioUI : ModuleRules
{
    public ModioUI(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.NoSharedPCHs;
        PrivatePCHHeaderFile = "Private/ModioUIPrivatePCH.h";
        bEnableUndefinedIdentifierWarnings = false;
        bEnforceIWYU = true;
        //bUseUnity = false;
        
        PublicDependencyModuleNames.AddRange(new string[] {
            "Core", "Engine", "UMG", "Slate", "SlateCore", "InputCore", "Modio", "PropertyPath", "RHI", "MSDFSupport"
			// ... add other public dependencies that you statically link with here ...
		});

        PrivateDependencyModuleNames.AddRange(new string[] {
            "CoreUObject", "RenderCore", "WidgetCarousel", "ApplicationCore", "AppFramework", "AssetRegistry"
			// ... add private dependencies that you statically link with here ...
		});

        DynamicallyLoadedModuleNames.AddRange(new string[] {
			// ... add any modules that your module loads dynamically here ...
		});

    }
}
