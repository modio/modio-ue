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
using System.IO;
using System.Collections.Generic;
using UnrealBuildTool;
using Tools.DotNETCommon;
using System.Linq;



public class Modio : ModuleRules
{
    private bool PlatformMatches(UnrealTargetPlatform PlatformToCheck, string PlatformIdentifier)
    {
        UnrealTargetPlatform Platform;
        bool ParsedSuccessfully = UnrealTargetPlatform.TryParse(PlatformIdentifier, out Platform);
        return ParsedSuccessfully && (PlatformToCheck == Platform);
    }
    public class ModioPlatformConfigFile
    {
        public class PlatformConfig
        {
            public List<string> IncludeDirectories = new List<string>();
            public List<string> PlatformSpecificDefines = new List<string>();
            public List<string> ModuleDependencies = new List<string>();
            public List<string> PlatformSourceFolderNames = new List<string>();
        }
        public Dictionary<string, PlatformConfig> Platforms;
        public List<string> IncludeDirectories;
        public List<string> PlatformSpecificDefines;
        public List<string> ModuleDependencies = new List<string>();
    };

    private void DumpNativePlatformConfig(ModioPlatformConfigFile.PlatformConfig Config)
    {
        foreach (string IncludeDirectory in Config.IncludeDirectories)
        {
            Log.TraceInformation("Include: " + IncludeDirectory);
        }
        foreach (string Define in Config.PlatformSpecificDefines)
        {
            Log.TraceInformation("Define: " + Define);
        }
        foreach (string SourceFolder in Config.PlatformSourceFolderNames)
        {
            Log.TraceInformation("Source Directory: " + SourceFolder);
        }
    }

    private ModioPlatformConfigFile.PlatformConfig LoadNativePlatformConfig()
    {
        ModioPlatformConfigFile.PlatformConfig MergedConfig = null;

        foreach (var PlatformDirectory in Directory.EnumerateDirectories(Path.Combine(ModuleDirectory, "../ThirdParty/NativeSDK/platform")))
        {
            string PlatformName = PlatformDirectory.Split(Path.DirectorySeparatorChar).Last();

            // Don't skip this - we have common directories that need merging too!
            /*if (PlatformName != PlatformString)
            {
                // Platforms that do not match the PlatformName are skipped
                // Log.TraceInformation("Platform name " + PlatformName + " does not match String " + PlatformString);
                continue;
            }*/

            if (File.Exists(Path.Combine(PlatformDirectory, "ueplatform.json")))
            {
                //Ensure changes to the platform jsons result in a rebuild of the plugin
                ExternalDependencies.Add(Path.Combine(PlatformDirectory, "ueplatform.json"));

                ModioPlatformConfigFile CurrentConfig = Json.Load<ModioPlatformConfigFile>(new FileReference(Path.Combine(PlatformDirectory, "ueplatform.json")));
                if (CurrentConfig != null)
                {
                    bool bFoundPlatformConfig = false;
                    foreach (KeyValuePair<string, ModioPlatformConfigFile.PlatformConfig> Platform in CurrentConfig.Platforms)
                    {
                        if (PlatformMatches(Target.Platform, Platform.Key))
                        {
                            Log.TraceInformation("Merging in platform configuration " + Platform.Key);
                            //This is the first matching platform we've found, so create our config
                            if (MergedConfig == null)
                            {
                                MergedConfig = new ModioPlatformConfigFile.PlatformConfig();
                            }

                            MergedConfig.IncludeDirectories.AddRange(Platform.Value.IncludeDirectories);
                            MergedConfig.PlatformSpecificDefines.AddRange(Platform.Value.PlatformSpecificDefines);
                            MergedConfig.ModuleDependencies.AddRange(Platform.Value.ModuleDependencies);

                            bFoundPlatformConfig = true;
                        }
                    }
                    //Only merge in the base data for this file, if one of the platforms inside it was a match
                    if (bFoundPlatformConfig)
                    {
                        MergedConfig.IncludeDirectories.AddRange(CurrentConfig.IncludeDirectories);
                        MergedConfig.PlatformSpecificDefines.AddRange(CurrentConfig.PlatformSpecificDefines);
                        MergedConfig.ModuleDependencies.AddRange(CurrentConfig.ModuleDependencies);
                    }
                }
            }
        }
        if (MergedConfig != null)
        {
            MergedConfig.PlatformSourceFolderNames = MergedConfig.IncludeDirectories;
            //Treat all platform specific includes as relative to the root native platform directory
            MergedConfig.IncludeDirectories =
                MergedConfig.IncludeDirectories.Select((string IncludeDir) => Path.Combine(ModuleDirectory, "../ThirdParty/NativeSDK/platform", IncludeDir)).ToList();
        }
        return MergedConfig;
    }

    private void CopyCommonGeneratedHeaders(string GeneratedHeaderPath)
    {
        // Only process generated headers if we are using the NativeSDK as a git submodule
        if (File.Exists(Path.Combine(ModuleDirectory, "../ThirdParty/NativeSDK/.git")))
        {
            //Clean the generated source directory so that we dont have any stale files in it
            if (Directory.Exists(GeneratedHeaderPath))
            {
                Directory.Delete(GeneratedHeaderPath, true);
            }
            Directory.CreateDirectory(GeneratedHeaderPath);

            Directory.CreateDirectory(Path.Combine(GeneratedHeaderPath, "Public"));
            Directory.CreateDirectory(Path.Combine(GeneratedHeaderPath, "Private"));

            //Because this file is a dummy we don't need it as a dependency
            string GeneratedHeaderFilePath = Path.Combine(GeneratedHeaderPath, "Private", "ModioGeneratedVariables.h");
            using (StreamWriter DummyGeneratedHeader = File.AppendText(GeneratedHeaderFilePath))
            { };

            // Silly hack/workaround until 4.26 adds ConditionalAddModuleDirectory - we may change where this lives in the native SDK later
            string ErrorConditionLibraryPath = Path.Combine(ModuleDirectory, "../ThirdParty/NativeSDK/modio/modio/core/ModioErrorCondition.h");
            // Add dependency on the upstream file so if it is modified we re-run and copy it again
            ExternalDependencies.Add(ErrorConditionLibraryPath);
            File.Copy(ErrorConditionLibraryPath, Path.Combine(GeneratedHeaderPath, "Public", "ModioErrorCondition.h"), true);
        }
    }

    /// <summary>
    /// Copies all platform implementation files to a platform-specific directory, renaming those files to cpp files so the UE build system will compile them
    /// </summary>
    /// <param name="GeneratedSourcePath"> The root directory containing the platform-specific source directories </param>
    private void CopyPlatformGeneratedSource(string GeneratedSourcePath)
    {
        if (File.Exists(Path.Combine(ModuleDirectory, "../ThirdParty/NativeSDK/.git")))
        {
            foreach (var PlatformDirectory in Directory.EnumerateDirectories(Path.Combine(ModuleDirectory, "../ThirdParty/NativeSDK/platform")))
            {
                string PlatformName = PlatformDirectory.Split(Path.DirectorySeparatorChar).Last();
                string PlatformGeneratedSourcePath = Path.Combine(GeneratedSourcePath, PlatformName);
                //Clean the generated source directory so that we dont have any stale files in it
                if (Directory.Exists(PlatformGeneratedSourcePath))
                {
                    Directory.Delete(PlatformGeneratedSourcePath, true);
                }
                Directory.CreateDirectory(PlatformGeneratedSourcePath);


                //copy all the ipp files to the right cpp directories
                List<string> IPPFiles = new List<string>(Directory.GetFiles(PlatformDirectory, "*.ipp", SearchOption.AllDirectories));

                foreach (string IPPFile in IPPFiles)
                {
                    //Add the original file in our upstream repository as a dependency, so if a user edits it we will copy it over
                    ExternalDependencies.Add(IPPFile);
                    string DestinationPath = Path.Combine(PlatformGeneratedSourcePath, Path.ChangeExtension(Path.GetFileName(IPPFile), ".cpp"));
                    File.Copy(IPPFile, DestinationPath, true);
                }
            }
        }
    }

    private void CopyCommonGeneratedSource(string GeneratedSourcePath)
    {
        string CommonGeneratedSourcePath = Path.Combine(GeneratedSourcePath, "core");
        if (File.Exists(Path.Combine(ModuleDirectory, "../ThirdParty/NativeSDK/.git")))
        {
            //Clean the generated source directory so that we dont have any stale files in it
            if (Directory.Exists(CommonGeneratedSourcePath))
            {
                Directory.Delete(CommonGeneratedSourcePath, true);
            }
            Directory.CreateDirectory(CommonGeneratedSourcePath);


            //copy all the ipp files to the right cpp directories
            List<string> IPPFiles = new List<string>(Directory.GetFiles(Path.Combine(ModuleDirectory, "../ThirdParty/NativeSDK/modio"), "*.ipp", SearchOption.AllDirectories));

            foreach (string IPPFile in IPPFiles)
            {
                //Add the original file in our upstream repository as a dependency, so if a user edits it we will copy it over
                ExternalDependencies.Add(IPPFile);
                string DestinationPath = Path.Combine(CommonGeneratedSourcePath, Path.ChangeExtension(Path.GetFileName(IPPFile), ".cpp"));
                File.Copy(IPPFile, DestinationPath, true);
            }
        }
    }

    private void AddCommonGeneratedSource(string GeneratedSourcePath)
    {
        ConditionalAddModuleDirectory(new DirectoryReference(Path.Combine(GeneratedSourcePath, "core")));
    }

    private void AddCommonHeaderPaths(string GeneratedHeaderPath)
    {
        PublicIncludePaths.AddRange(new string[] {
                Path.Combine(GeneratedHeaderPath, "Public")
                });

        // Add common private includes from the Native SDK
        PrivateIncludePaths.AddRange(new string[]
        {
                Path.Combine(ModuleDirectory,  "../ThirdParty/NativeSDK/ext/json/single_include"),
                Path.Combine(ModuleDirectory,  "../ThirdParty/NativeSDK/ext/filesystem/include"),
                Path.Combine(ModuleDirectory,  "../ThirdParty/NativeSDK/ext/fmt/include"),
                Path.Combine(ModuleDirectory,  "../ThirdParty/NativeSDK/ext/optional/include"),
                Path.Combine(ModuleDirectory,  "../ThirdParty/NativeSDK/ext/asio/asio/include"),
                Path.Combine(ModuleDirectory,  "../ThirdParty/NativeSDK/ext/utfcpp/source"),
                Path.Combine(ModuleDirectory,  "../ThirdParty/NativeSDK/ext/httpparser/src"),
                Path.Combine(ModuleDirectory,  "../ThirdParty/NativeSDK/platform/interface"),
                Path.Combine(ModuleDirectory,  "../ThirdParty/NativeSDK/modio"),
                Path.Combine(GeneratedHeaderPath, "Private"),
                Path.Combine(GeneratedHeaderPath, "Public")
        });


    }

    private void AddCommonDependencyModules()
    {
        PublicDependencyModuleNames.AddRange(new string[] {
            "Core", "Engine", "UMG", "Slate"
        });

        PrivateDependencyModuleNames.AddRange(new string[] {
            "Projects", "CoreUObject", "RHI", "RenderCore", "HTTP"
        });
    }

    private void ApplyNativePlatformConfig(ModioPlatformConfigFile.PlatformConfig Config, string GeneratedHeaderPath, string GeneratedSourcePath)
    {
        // Add only the source directory for the current platform to this module
        // Currently this is the portion which is not compatible with 4.25
        foreach (string PlatformPath in Config.PlatformSourceFolderNames)
        {
            string RootPlatformName = PlatformPath.Replace('/', Path.DirectorySeparatorChar).Split(Path.DirectorySeparatorChar).First();
            ConditionalAddModuleDirectory(new DirectoryReference(Path.Combine(GeneratedSourcePath, RootPlatformName)));
            Log.TraceInformation("Adding native platform source directory " + Path.Combine(GeneratedSourcePath, RootPlatformName));
        }
        // Add any platform-specific additional modules as private dependencies
        PrivateDependencyModuleNames.AddRange(Config.ModuleDependencies.ToArray());

        // Add platform-specific compiler defines
        foreach (string CompilerDefine in Config.PlatformSpecificDefines)
        {
            PrivateDefinitions.Add(CompilerDefine);
        }
        // Add platform-specific include directories
        PrivateIncludePaths.AddRange(Config.IncludeDirectories);
    }
    private void AddCommonDefinitions()
    {
        if (Target.Configuration == UnrealTargetConfiguration.Debug ||
    Target.Configuration == UnrealTargetConfiguration.DebugGame)
        {
            PrivateDefinitions.Add("MODIO_DEBUG=1");
            PrivateDefinitions.Add("MODIO_RELEASE=0");
        }
        else
        {
            PrivateDefinitions.Add("MODIO_DEBUG=0");
            PrivateDefinitions.Add("MODIO_RELEASE=1");
        }

        // Enable Unreal-specific headers in the native SDK
        PrivateDefinitions.Add("MODIO_PLATFORM_UNREAL");
        // Disable header-only mode
        PrivateDefinitions.Add("MODIO_SEPARATE_COMPILATION=1");
        // Disable unnecessary inlining as a result of header-only mode not being used
        PrivateDefinitions.Add("MODIO_IMPL=");

        // Pass-through of SDK version identifier with Unreal prefix
        string VersionFilePath = Path.Combine(ModuleDirectory, "../ThirdParty/NativeSDK/.modio");
        string VersionString = File.ReadAllText(VersionFilePath);
        VersionString = VersionString.Trim();
        PrivateDefinitions.Add(string.Format("MODIO_COMMIT_HASH=\"UNREAL-{0}\"", VersionString));

        // Add dependency on version file so if it is changed we trigger a rebuild
        ExternalDependencies.Add(VersionFilePath);
    }

    public Modio(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.NoSharedPCHs;
        PrivatePCHHeaderFile = "Private/ModioPrivatePCH.h";
        bEnableUndefinedIdentifierWarnings = false;
        bEnforceIWYU = true;
        //bUseUnity = false;
        bAllowConfidentialPlatformDefines = true;
        string GeneratedSourcePath = Path.Combine(ModuleDirectory, "../ThirdParty/GeneratedNativeSDK", "GeneratedSource");
        string GeneratedHeaderPath = Path.Combine(ModuleDirectory, "../ThirdParty/GeneratedNativeSDK", "GeneratedHeader");

        if (!Directory.Exists(Path.Combine(ModuleDirectory, "../ThirdParty/GeneratedNativeSDK")))
        {
            Directory.CreateDirectory(Path.Combine(ModuleDirectory, "../ThirdParty/GeneratedNativeSDK"));
        }

        if (!Directory.Exists(GeneratedSourcePath))
        {
            Directory.CreateDirectory(GeneratedSourcePath);
        }

        if (!Directory.Exists(GeneratedHeaderPath))
        {
            Directory.CreateDirectory(GeneratedHeaderPath);
        }

        // This should be moved into UEPlatform.json handling
        if (Target.Platform == UnrealTargetPlatform.Linux)
        {
            // Currently required because Clang is picky and MSVC not, and tl::optional doesn't have a nothrow implementation
            bEnableExceptions = true;
        }

        

        ModioPlatformConfigFile.PlatformConfig PlatformConfig = LoadNativePlatformConfig();

        if (PlatformConfig == null)
        {
            throw new BuildException("Could not locate native platform configuration file. If you are using a confidential platform, please ensure you have placed the platform code into the correct directory.");
        }

        DumpNativePlatformConfig(PlatformConfig);

        // When using the native SDK as a submodule, transform the SDK source into paths and files that UBT understands
        // These are all no-ops in marketplace builds or anywhere with 'baked' source files
        CopyCommonGeneratedSource(GeneratedSourcePath);
        CopyPlatformGeneratedSource(GeneratedSourcePath);
        CopyCommonGeneratedHeaders(GeneratedHeaderPath);

        // Configure the module with the common headers, modules and compiler defines
        AddCommonHeaderPaths(GeneratedHeaderPath);
        AddCommonGeneratedSource(GeneratedSourcePath);
        AddCommonDependencyModules();
        AddCommonDefinitions();

        // Apply all platform-specific includes, source, modules, defines, etc
        ApplyNativePlatformConfig(PlatformConfig, GeneratedHeaderPath, GeneratedSourcePath);

    }
}
