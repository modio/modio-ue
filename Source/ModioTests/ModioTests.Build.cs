/* 
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *  
 *  This file is part of the mod.io UE Plugin.
 *  
 *  Distributed under the MIT License. (See accompanying file LICENSE or 
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *   
 */

// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
using System.IO;
using System.Collections.Generic;
using UnrealBuildTool;

public class ModioTests : ModuleRules
{
    public ModioTests(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.NoSharedPCHs;
		PrivatePCHHeaderFile = "Private/ModioTestsPrivatePCH.h";

#if UE_5_3_OR_LATER
	    IWYUSupport = IWYUSupport.Full;
	    CppStandard = CppStandardVersion.Cpp17;
#else
		bEnforceIWYU = true;
#endif
        {
           
            PublicIncludePaths.AddRange(new string[] {
                });

            PublicSystemIncludePaths.AddRange(new string[] {
                });

            PrivateIncludePaths.AddRange(new string[]
            {
            });
        }
        

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core", "Engine"
			// ... add other public dependencies that you statically link with here ...
		});

        PrivateDependencyModuleNames.AddRange(new string[] {
            "Projects", "CoreUObject", "RHI", "RenderCore", "HTTP", "Modio"
			// ... add private dependencies that you statically link with here ...
		});

        DynamicallyLoadedModuleNames.AddRange(new string[] {
			// ... add any modules that your module loads dynamically here ...
		});

    }
}
