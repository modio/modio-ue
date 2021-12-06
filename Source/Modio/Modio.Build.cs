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
        }
        public Dictionary<string, PlatformConfig> Platforms;
        public List<string> IncludeDirectories;
        public List<string> PlatformSpecificDefines;
        public List<string> ModuleDependencies = new List<string>();
    };

    private void DumpNativePlatformConfig(ModioPlatformConfigFile.PlatformConfig Config)
    {
        foreach(string IncludeDirectory in Config.IncludeDirectories)
        {
            Log.TraceInformation("Include: " + IncludeDirectory);
        }
        foreach (string Define in Config.PlatformSpecificDefines)
        {
            Log.TraceInformation("Define: " + Define);
        }
    }

    private ModioPlatformConfigFile.PlatformConfig LoadNativePlatformConfig()
    {
        ModioPlatformConfigFile.PlatformConfig MergedConfig = null;
        
        foreach (var PlatformDirectory in Directory.EnumerateDirectories(Path.Combine(ModuleDirectory, "../ThirdParty/NativeSDK/platform")))
        {
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
            //Treat all platform specific includes as relative to the root native platform directory
            MergedConfig.IncludeDirectories =
                MergedConfig.IncludeDirectories.Select((string IncludeDir) => Path.Combine(ModuleDirectory, "../ThirdParty/NativeSDK/platform", IncludeDir)).ToList();
        }
        return MergedConfig;
    }

    public Modio(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.NoSharedPCHs;
        PrivatePCHHeaderFile = "Private/ModioPrivatePCH.h";
        bEnableUndefinedIdentifierWarnings = false;
        bEnforceIWYU = true;
        bAllowConfidentialPlatformDefines = true;
        //bUseUnity = false;
        ModioPlatformConfigFile.PlatformConfig PlatformConfig = LoadNativePlatformConfig();

        if (PlatformConfig == null)
        {
            throw new BuildException("Could not locate native platform configuration file. If you are using a confidential platform, please ensure you have placed the platform code into the correct directory.");
        }
        DumpNativePlatformConfig(PlatformConfig);
        //Add stub generated header
        {
            string GeneratedHeaderPath = Path.Combine(ModuleDirectory, "GeneratedHeader");

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


            PublicIncludePaths.AddRange(new string[] {
                Path.Combine(GeneratedHeaderPath, "Public")
                });

            PublicSystemIncludePaths.AddRange(new string[] {
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

            //Import private platform-specific includes from the Native SDK
            PrivateIncludePaths.AddRange(PlatformConfig.IncludeDirectories);

        }

        // Add native SDK implementation to this module so we don't have to create an extraneous module
        // TODO: @modio-ue4 cleanup by using UE_4_26_OR_LATER so we can use ConditionalAddModuleDirectory

        // We only need to check and update generated source if we are consuming the NativeSDK as a git submodule (ie we aren't in a standalone release context)
        if (File.Exists(Path.Combine(ModuleDirectory, "../ThirdParty/NativeSDK/.git")))
        {
            string GeneratedSourcePath = Path.Combine(ModuleDirectory, "GeneratedSource");

            //Clean the generated source directory so that we dont have any stale files in it
            if (Directory.Exists(GeneratedSourcePath))
            {
                Directory.Delete(GeneratedSourcePath, true);
            }
            Directory.CreateDirectory(GeneratedSourcePath);

            //Copy all implementation headers (ipp files) to a generated source directory with the cpp extension so they will get compiled/linked
            {

                List<string> IPPFiles = new List<string>(Directory.GetFiles(Path.Combine(ModuleDirectory, "../ThirdParty/NativeSDK/modio"), "*.ipp", SearchOption.AllDirectories));

                // Add platform-specific include directories
                foreach (string IncludePath in PlatformConfig.IncludeDirectories)
                {
                    IPPFiles.AddRange(Directory.GetFiles(IncludePath, "*.ipp", SearchOption.AllDirectories));
                }

                foreach (string IPPFile in IPPFiles)
                {
                    //Add the original file in our upstream repository as a dependency, so if a user edits it we will copy it over
                    ExternalDependencies.Add(IPPFile);
                    string DestinationPath = Path.Combine(GeneratedSourcePath, Path.ChangeExtension(Path.GetFileName(IPPFile), ".cpp"));
                    File.Copy(IPPFile, DestinationPath, true);
                }
            }

            //Only supported from 4.26 onwards - workaround at the moment is for the GeneratedSourcePath to live inside the module directory
            //Once 4.25 support is dropped, generated source should be in the Intermediate folder
            //ConditionalAddModuleDirectory(new Tools.DotNETCommon.DirectoryReference(GeneratedSourcePath));
        }


        PublicDependencyModuleNames.AddRange(new string[] {
            "Core", "Engine", "UMG", "Slate"
			// ... add other public dependencies that you statically link with here ...
		});

        PrivateDependencyModuleNames.AddRange(new string[] {
            "Projects", "CoreUObject", "RHI", "RenderCore", "HTTP"
			// ... add private dependencies that you statically link with here ...
		});

        PrivateDependencyModuleNames.AddRange(PlatformConfig.ModuleDependencies.ToArray());

        DynamicallyLoadedModuleNames.AddRange(new string[] {
			// ... add any modules that your module loads dynamically here ...
		});

        // Set native SDK preprocessor defines
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

            foreach (string CompilerDefine in PlatformConfig.PlatformSpecificDefines)
            {
                PrivateDefinitions.Add(CompilerDefine);
            }

            // Pass-through of SDK version identifier with Unreal prefix
            string VersionFilePath = Path.Combine(ModuleDirectory, "../ThirdParty/NativeSDK/.modio");
            string VersionString = File.ReadAllText(VersionFilePath);
            VersionString = VersionString.Trim();
            PrivateDefinitions.Add(string.Format("MODIO_COMMIT_HASH=\"UNREAL-{0}\"", VersionString));

            // Add dependency on version file so if it is changed we trigger a rebuild
            ExternalDependencies.Add(VersionFilePath);
        }

    }
}
