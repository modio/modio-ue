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
        //Required for msdfgen source to compile
        bUseUnity = false;
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

        PublicDefinitions.Add("MSDFGEN_NO_RTTI");

        // Standard include paths
        PublicIncludePaths.AddRange(new string[] { Path.Combine(ModuleDirectory, "Public") });
        PrivateIncludePaths.AddRange(new string[] { Path.Combine(ModuleDirectory, "Private") });

        string GeneratedSourcePath = Path.Combine(ModuleDirectory, "../ThirdParty/GeneratedMSDFgen", "GeneratedSource");
        string GeneratedHeaderPath = Path.Combine(ModuleDirectory, "../ThirdParty/GeneratedMSDFgen", "GeneratedHeader");

        // msdfgen source code requires a number of warnings to be squashed. We do this by wrapping the cpp files with our own preamble and epilogue.
        // In addition to importing modified source files, we also pull the msdfgen headers into our own tree, so that if the plugin is packaged, we don't need the submodule.
        // Only do the regeneration of the generated sources if the msdfgen code is present as a submodule, otherwise we assume its static code that will never change
        CopyCommonGeneratedSource(GeneratedSourcePath);
        CopyCommonGeneratedHeaders(GeneratedHeaderPath);
        AddCommonGeneratedSource(GeneratedSourcePath);
        AddCommonGeneratedHeaders(GeneratedHeaderPath);

    }
    public void AddCommonGeneratedHeaders(string GeneratedHeaderPath)
    {
        PrivateIncludePaths.AddRange(new string[] { GeneratedHeaderPath, Path.Combine(GeneratedHeaderPath, "core"),
                Path.Combine(GeneratedHeaderPath, "include"), Path.Combine(GeneratedHeaderPath, "ext") });

    }
    public void AddCommonGeneratedSource(string GeneratedSourcePath)
    {
        ConditionalAddModuleDirectory(new DirectoryReference(GeneratedSourcePath));
    }
    public void CopyCommonGeneratedSource(string GeneratedSourcePath)
    {
        if (File.Exists(Path.Combine(ModuleDirectory, "../ThirdParty/msdfgen/.git")) || Directory.Exists(Path.Combine(ModuleDirectory, "../ThirdParty/msdfgen/.git")))
        {
            //Clean the generated source directory so that we dont have any stale files in it
            if (Directory.Exists(GeneratedSourcePath))
            {
                Directory.Delete(GeneratedSourcePath, true);
            }
            Directory.CreateDirectory(GeneratedSourcePath);

            {

                List<string> CPPFiles = new List<string>(Directory.GetFiles(Path.Combine(ModuleDirectory, "../ThirdParty/msdfgen/lib"), "*.cpp", SearchOption.AllDirectories));
                CPPFiles.AddRange(Directory.GetFiles(Path.Combine(ModuleDirectory, "../ThirdParty/msdfgen/core"), "*.cpp", SearchOption.AllDirectories));
                CPPFiles.AddRange(Directory.GetFiles(Path.Combine(ModuleDirectory, "../ThirdParty/msdfgen/ext"), "*.cpp", SearchOption.AllDirectories));
                CPPFiles.AddRange(Directory.GetFiles(Path.Combine(ModuleDirectory, "../ThirdParty/msdfgen/include"), "*.cpp", SearchOption.AllDirectories));


                foreach (string CPPFile in CPPFiles)
                {
                    //Add the original file in our upstream repository as a dependency, so if a user edits it we will copy it over
                    ExternalDependencies.Add(CPPFile);
                    string DestinationPath = Path.Combine(GeneratedSourcePath, Path.GetFileName(CPPFile));
                    //string FileContents = "\n\n#pragma warning(push)\n#pragma warning(disable : 4267)\n#pragma warning(disable : 4127)\n" +
                    //    "#pragma warning(disable : 4456)\n#pragma warning(disable : 4706)\n#pragma warning(disable : 4458)\n" +
                    //    "#pragma warning(disable : 4100)\n#pragma warning(disable : 4505)\n#pragma warning(disable : 4457)\n#pragma warning(disable : 4996)\n";

                    string FileContents = "#include \"HAL/Platform.h\"\nTHIRD_PARTY_INCLUDES_START\n#pragma warning(push)\n#pragma warning(disable : 4706)\n";
                    FileContents += File.ReadAllText(CPPFile);
                    FileContents += "\n#pragma warning(pop)\n";
                    FileContents += "\nTHIRD_PARTY_INCLUDES_END\n";
                    File.WriteAllText(DestinationPath, FileContents);
                }
            }
        }
    }
    public void CopyCommonGeneratedHeaders(string GeneratedHeaderPath)
    {
        if (File.Exists(Path.Combine(ModuleDirectory, "../ThirdParty/msdfgen/.git")) || Directory.Exists(Path.Combine(ModuleDirectory, "../ThirdParty/msdfgen/.git")))
        {
            //Clean the generated header directory so that we dont have any stale files in it
            if (Directory.Exists(GeneratedHeaderPath))
            {
                Directory.Delete(GeneratedHeaderPath, true);
            }
            Directory.CreateDirectory(GeneratedHeaderPath);

            {

                List<string> HeaderFiles = new List<string>(Directory.GetFiles(Path.Combine(ModuleDirectory, "../ThirdParty/msdfgen/lib"), "*.h*", SearchOption.AllDirectories));
                HeaderFiles.AddRange(Directory.GetFiles(Path.Combine(ModuleDirectory, "../ThirdParty/msdfgen/core"), "*.h*", SearchOption.AllDirectories));
                HeaderFiles.AddRange(Directory.GetFiles(Path.Combine(ModuleDirectory, "../ThirdParty/msdfgen/ext"), "*.h*", SearchOption.AllDirectories));
                HeaderFiles.AddRange(Directory.GetFiles(Path.Combine(ModuleDirectory, "../ThirdParty/msdfgen/include"), "*.h*", SearchOption.AllDirectories));
                HeaderFiles.AddRange(Directory.GetFiles(Path.Combine(ModuleDirectory, "../ThirdParty/msdfgen/"), "*.h*", SearchOption.TopDirectoryOnly));


                foreach (string HeaderFile in HeaderFiles)
                {
                    //Add the original file in our upstream repository as a dependency, so if a user edits it we will copy it over
                    ExternalDependencies.Add(HeaderFile);
                    //Preserve subdirectory structure (msdfgen includes headers via relative paths in a few places, grumble grumble)
                    string RelativeDestinationPath = HeaderFile.Replace(Path.Combine(ModuleDirectory, "../ThirdParty/msdfgen/"), "");
                    string DestinationPath = Path.Combine(GeneratedHeaderPath, RelativeDestinationPath);
                    Directory.CreateDirectory(Path.GetDirectoryName(DestinationPath) + "/");
                    //copy the header
                    File.Copy(HeaderFile, DestinationPath);
                }
            }

        }
    }

}