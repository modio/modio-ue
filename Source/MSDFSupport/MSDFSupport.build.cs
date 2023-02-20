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
#if UE_5_0_OR_LATER
using EpicGames.Core;
#else
using Tools.DotNETCommon;
#endif
using System.IO;
using System.Collections.Generic;

public class MSDFSupport : ModuleRules
{
    public MSDFSupport(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.NoPCHs;
        //bUseRTTI = true;
        bEnableExceptions = true;

        // Module dependencies
        PrivateDependencyModuleNames.AddRange(new string[] { "FreeType2", "UElibPNG", "zlib" });
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine" });

        // Private definitions
        if (Target.Platform != UnrealTargetPlatform.Mac)
        {
            // Mac compilation already defines this macro
            PrivateDefinitions.Add("M_PI=3.14159265358979323846");
        }

        // Standard include paths
        PublicIncludePaths.AddRange(new string[] { Path.Combine(ModuleDirectory, "Public") });
        PrivateIncludePaths.AddRange(new string[] { Path.Combine(ModuleDirectory, "Private") });

        string GeneratedSourcePath = Path.Combine(ModuleDirectory, "../ThirdParty/GeneratedMSDFgen", "GeneratedSource");
        string GeneratedHeaderPath = Path.Combine(ModuleDirectory, "../ThirdParty/GeneratedMSDFgen", "GeneratedHeader");
    }

}