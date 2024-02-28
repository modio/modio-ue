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

//#define ENABLE_TRACE_LOG

using System;
using System.IO;
using System.Collections.Generic;
using UnrealBuildTool;
#if UE_5_0_OR_LATER
using EpicGames.Core;
using Microsoft.Extensions.Logging;
#else
using Tools.DotNETCommon;
#endif
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
            public List<string> SystemLibraryDependencies = new List<string>();
            public List<string> PlatformSourceFolderNames = new List<string>();
        }
        public Dictionary<string, PlatformConfig> Platforms;
        public List<string> IncludeDirectories;
        public List<string> PlatformSpecificDefines;
        public List<string> ModuleDependencies = new List<string>();
        public List<string> SystemLibraryDependencies = new List<string>();
    };
#if UE_5_0_OR_LATER
    private ModioPlatformConfigFile.PlatformConfig ParseInnerPlatformConfig(JsonObject InnerPlatformObject)
    {
        ModioPlatformConfigFile.PlatformConfig ParsedInnerPlatform = new ModioPlatformConfigFile.PlatformConfig();
        string[] ParsedIncludeDirectories;
        if (InnerPlatformObject.TryGetStringArrayField("IncludeDirectories", out ParsedIncludeDirectories))
        {
            ParsedInnerPlatform.IncludeDirectories = new List<string>(ParsedIncludeDirectories);
        }
        string[] ParsedPlatformSpecificDefines;
        if (InnerPlatformObject.TryGetStringArrayField("PlatformSpecificDefines", out ParsedPlatformSpecificDefines))
        {
            ParsedInnerPlatform.PlatformSpecificDefines = new List<string>(ParsedPlatformSpecificDefines);
        }
        string[] ParsedModuleDependencies;
        if (InnerPlatformObject.TryGetStringArrayField("ModuleDependencies", out ParsedModuleDependencies))
        {
            ParsedInnerPlatform.ModuleDependencies = new List<string>(ParsedModuleDependencies);
        }
        string[] ParsedPlatformSourceFolderNames;
        if (InnerPlatformObject.TryGetStringArrayField("PlatformSourceFolderNames", out ParsedPlatformSourceFolderNames))
        {
            ParsedInnerPlatform.PlatformSourceFolderNames = new List<string>(ParsedPlatformSourceFolderNames);
        }
        string[] ParsedPlatformSystemLibraryDependencies;
        if (InnerPlatformObject.TryGetStringArrayField("SystemLibraryDependencies", out ParsedPlatformSystemLibraryDependencies))
        {
            ParsedInnerPlatform.SystemLibraryDependencies = new List<string>(ParsedPlatformSystemLibraryDependencies);
        }
        return ParsedInnerPlatform;
    }
#endif

    
    private ModioPlatformConfigFile TryLoadPlatformConfig(string PlatformConfigPath)
    {
#if UE_5_0_OR_LATER
        ModioPlatformConfigFile ParsedConfig = new ModioPlatformConfigFile();

        JsonObject PlatformConfigObject = JsonObject.Read(new FileReference(PlatformConfigPath));
        string[] ParsedModuleDependencies;
        if (PlatformConfigObject.TryGetStringArrayField("ModuleDependencies", out ParsedModuleDependencies))
        {
            ParsedConfig.ModuleDependencies = new List<string>(ParsedModuleDependencies);
        }
        ParsedConfig.IncludeDirectories = new List<string>(PlatformConfigObject.GetStringArrayField("IncludeDirectories"));
        ParsedConfig.PlatformSpecificDefines = new List<string>(PlatformConfigObject.GetStringArrayField("PlatformSpecificDefines"));
        string[] ParsedPlatformSystemLibraryDependencies;
        if (PlatformConfigObject.TryGetStringArrayField("SystemLibraryDependencies", out ParsedPlatformSystemLibraryDependencies))
        {
            ParsedConfig.SystemLibraryDependencies = new List<string>(ParsedPlatformSystemLibraryDependencies);
        }

        JsonObject PlatformsInnerObject = PlatformConfigObject.GetObjectField("Platforms");
        ParsedConfig.Platforms = PlatformsInnerObject.KeyNames.ToDictionary(x => x, x =>
        ParseInnerPlatformConfig(PlatformsInnerObject.GetObjectField(x)), System.StringComparer.OrdinalIgnoreCase);
        return ParsedConfig;
#else
        return Json.Load<ModioPlatformConfigFile>(new FileReference(PlatformConfigPath));
#endif
    }

    private void InternalLog(string message)
    {
#if UE_5_3_OR_LATER
	    Logger.LogInformation(message);
        #else
        Log.TraceInformation(message);
#endif
    }

	private void DumpNativePlatformConfig(ModioPlatformConfigFile.PlatformConfig Config)
    {
        foreach (string IncludeDirectory in Config.IncludeDirectories)
        {
	        InternalLog("Include: " + IncludeDirectory);
        }
        foreach (string Define in Config.PlatformSpecificDefines)
        {
	        InternalLog("Define: " + Define);
        }
        foreach (string SourceFolder in Config.PlatformSourceFolderNames)
        {
	        InternalLog("Source Directory: " + SourceFolder);
        }
        foreach (string ModuleName in Config.ModuleDependencies)
        {
            InternalLog("Platform Module: " + ModuleName);
        }
        foreach (string SystemLibrary in Config.SystemLibraryDependencies)
        {
	        InternalLog("System Library: " + SystemLibrary);
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
                // #if ENABLE_TRACE_LOG
                //     Log.TraceInformation("Platform name " + PlatformName + " does not match String " + PlatformString);
                // #endif
                continue;
            }*/

            if (File.Exists(Path.Combine(PlatformDirectory, "ueplatform.json")))
            {
                //Ensure changes to the platform jsons result in a rebuild of the plugin
                ExternalDependencies.Add(Path.Combine(PlatformDirectory, "ueplatform.json"));

                ModioPlatformConfigFile CurrentConfig = TryLoadPlatformConfig(Path.Combine(PlatformDirectory, "ueplatform.json"));
                if (CurrentConfig != null)
                {
                    bool bFoundPlatformConfig = false;
                    foreach (KeyValuePair<string, ModioPlatformConfigFile.PlatformConfig> Platform in CurrentConfig.Platforms)
                    {
                        if (PlatformMatches(Target.Platform, Platform.Key))
                        {
#if ENABLE_TRACE_LOG
                            InternalLog("Merging in platform configuration " + Platform.Key);
#endif

							//This is the first matching platform we've found, so create our config
							if (MergedConfig == null)
                            {
                                MergedConfig = new ModioPlatformConfigFile.PlatformConfig();
                            }

                            MergedConfig.IncludeDirectories.AddRange(Platform.Value.IncludeDirectories);
                            MergedConfig.PlatformSpecificDefines.AddRange(Platform.Value.PlatformSpecificDefines);
                            MergedConfig.ModuleDependencies.AddRange(Platform.Value.ModuleDependencies);
                            MergedConfig.SystemLibraryDependencies.AddRange(Platform.Value.SystemLibraryDependencies);
                            bFoundPlatformConfig = true;
                        }
                    }
                    //Only merge in the base data for this file, if one of the platforms inside it was a match
                    if (bFoundPlatformConfig)
                    {
                        MergedConfig.IncludeDirectories.AddRange(CurrentConfig.IncludeDirectories);
                        MergedConfig.PlatformSpecificDefines.AddRange(CurrentConfig.PlatformSpecificDefines);
                        MergedConfig.ModuleDependencies.AddRange(CurrentConfig.ModuleDependencies);
                        MergedConfig.SystemLibraryDependencies.AddRange(CurrentConfig.SystemLibraryDependencies);
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

    public class ModioTestConfigFile
    {
        public class FileToCopy
        {
            public string SourcePath;
            public string DestPath;
        }
        public List<string> TestDefines = new List<string>();
        public List<FileToCopy> TestFiles = new List<FileToCopy>();
    }

    private ModioTestConfigFile LoadTestConfig()
    {
        string TestConfigPath = Path.Combine(ModuleDirectory, "../ThirdParty/NativeSDK/tests", "UnrealTestConfig.json"); 
        if (File.Exists(TestConfigPath))
        {
#if UE_5_0_OR_LATER
            ModioTestConfigFile ParsedConfig = new ModioTestConfigFile();

            JsonObject TestConfigObject = JsonObject.Read(new FileReference(TestConfigPath));
            string[] ParsedTestDefines;
            if (TestConfigObject.TryGetStringArrayField("TestDefines", out ParsedTestDefines))
            {
                ParsedConfig.TestDefines = new List<string>(ParsedTestDefines);
            }
            JsonObject[] TestFiles;
            if (TestConfigObject.TryGetObjectArrayField("TestFiles", out TestFiles))
            {
                foreach (JsonObject TestFile in TestFiles)
                {
                    ModioTestConfigFile.FileToCopy CurrentFileToCopy = new ModioTestConfigFile.FileToCopy();
                    CurrentFileToCopy.SourcePath = TestFile.GetStringField("SourcePath");
                    CurrentFileToCopy.DestPath = TestFile.GetStringField("DestPath");
                    ParsedConfig.TestFiles.Add(CurrentFileToCopy);
                }
            }
            return ParsedConfig;
#else
            return Json.Load<ModioTestConfigFile>(new FileReference(TestConfigPath));
#endif
        }
        else
        {
#if ENABLE_TRACE_LOG
        InternalLog("TestConfig.json not found. Skipping unit tests");
#endif
			return null;
        }
    }

    private void ApplyTestConfig(string GeneratedHeaderPath, string GeneratedSourcePath)
    {
        ModioTestConfigFile TestConfig = LoadTestConfig();
        if (TestConfig != null)
        {
            foreach (string CompilerDefine in TestConfig.TestDefines)
            {
#if ENABLE_TRACE_LOG
                InternalLog("Testing Define: " + CompilerDefine);
#endif
				PrivateDefinitions.Add(CompilerDefine);
            }
            foreach(ModioTestConfigFile.FileToCopy TestFile in TestConfig.TestFiles)
            {
                Directory.CreateDirectory(Path.Combine(GeneratedHeaderPath, Path.GetDirectoryName(TestFile.DestPath)));
                File.Copy(Path.Combine(ModuleDirectory, "../ThirdParty/NativeSDK", TestFile.SourcePath), Path.Combine(GeneratedHeaderPath, TestFile.DestPath));
            }
        }
    }

    private void CopyCommonGeneratedHeaders(string GeneratedHeaderPath)
    {
        // Only process generated headers if we are using the NativeSDK as a git submodule
        if (Directory.Exists(Path.Combine(ModuleDirectory, "../ThirdParty/NativeSDK/.git")) || File.Exists(Path.Combine(ModuleDirectory, "../ThirdParty/NativeSDK/.git")))
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
        if (Directory.Exists(Path.Combine(ModuleDirectory, "../ThirdParty/NativeSDK/.git")) || File.Exists(Path.Combine(ModuleDirectory, "../ThirdParty/NativeSDK/.git")))
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
        if (Directory.Exists(Path.Combine(ModuleDirectory, "../ThirdParty/NativeSDK/.git")) || File.Exists(Path.Combine(ModuleDirectory, "../ThirdParty/NativeSDK/.git")))
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
        ConditionalAddModuleDirectory(new DirectoryReference(Path.Combine(GeneratedHeaderPath, "Public")));
        // Add common private includes from the Native SDK
        PrivateIncludePaths.AddRange(new string[]
        {
                Path.Combine(ModuleDirectory,  "../ThirdParty/NativeSDK/ext/json/single_include"),
                Path.Combine(ModuleDirectory,  "../ThirdParty/NativeSDK/ext/filesystem/include"),
                Path.Combine(ModuleDirectory,  "../ThirdParty/NativeSDK/ext/function2/include"),
                Path.Combine(ModuleDirectory,  "../ThirdParty/NativeSDK/ext/fmt/include"),
                Path.Combine(ModuleDirectory,  "../ThirdParty/NativeSDK/ext/optional/include"),
                Path.Combine(ModuleDirectory,  "../ThirdParty/NativeSDK/ext/asio/asio/include"),
                Path.Combine(ModuleDirectory,  "../ThirdParty/NativeSDK/ext/utfcpp/source"),
                Path.Combine(ModuleDirectory,  "../ThirdParty/NativeSDK/ext/httpparser/src"),
                Path.Combine(ModuleDirectory,  "../ThirdParty/NativeSDK/platform/interface"),
                Path.Combine(ModuleDirectory,  "../ThirdParty/NativeSDK/modio"),
                Path.Combine(ModuleDirectory,  "../ThirdParty/concurrentqueue"),
 
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

        if (Target.bBuildEditor)
        {
            PrivateDependencyModuleNames.Add("UnrealEd");
        }
    }

    private void ApplyNativePlatformConfig(ModioPlatformConfigFile.PlatformConfig Config, string GeneratedHeaderPath, string GeneratedSourcePath)
    {
        // Add only the source directory for the current platform to this module
        // Currently this is the portion which is not compatible with 4.25
        foreach (string PlatformPath in Config.PlatformSourceFolderNames)
        {
            string RootPlatformName = PlatformPath.Replace('/', Path.DirectorySeparatorChar).Split(Path.DirectorySeparatorChar).First();
            ConditionalAddModuleDirectory(new DirectoryReference(Path.Combine(GeneratedSourcePath, RootPlatformName)));

#if ENABLE_TRACE_LOG
            InternalLog("Adding native platform source directory " + Path.Combine(GeneratedSourcePath, RootPlatformName));
#endif
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
        PublicSystemLibraries.AddRange(Config.SystemLibraryDependencies);
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

        // Enable Modio Profiling in non-shipping builds
        if (Target.Configuration != UnrealTargetConfiguration.Shipping && Target.Configuration != UnrealTargetConfiguration.Test && DoesPlatformSupportProfiling(Target.Platform))
        {
            PrivateDefinitions.Add("MODIO_ENABLE_PROFILING=1");
            PrivateDefinitions.Add("MODIO_UNREAL_PROFILING_SUPPORT=1");
        }

        // Enable Unreal-specific headers in the native SDK
        PrivateDefinitions.Add("MODIO_PLATFORM_UNREAL");
        // Disable header-only mode
        PrivateDefinitions.Add("MODIO_SEPARATE_COMPILATION=1");
        // Disable unnecessary inlining as a result of header-only mode not being used
        PrivateDefinitions.Add("MODIO_IMPL=");
        // Disable internal warnings caused by deprecated functionality we don't consume
        PrivateDefinitions.Add("MODIO_DISABLE_ALL_DEPRECATIONS");

        // Pass-through of SDK version identifier with Unreal prefix
        string VersionFilePath = Path.Combine(ModuleDirectory, "../ThirdParty/NativeSDK/.modio");
        string VersionString = File.ReadAllText(VersionFilePath);
        VersionString = VersionString.Trim();
        PrivateDefinitions.Add(string.Format("MODIO_COMMIT_HASH=\"UNREAL-{0}\"", VersionString));

        // Add dependency on version file so if it is changed we trigger a rebuild
        ExternalDependencies.Add(VersionFilePath);
    }

    private bool DoesPlatformSupportProfiling(UnrealTargetPlatform Platform)
    {
        // Only enable on Windows for now, we may want to modify this later for console support as well
        if (Platform == UnrealTargetPlatform.Win64)
        {
            return true;
        }

        return false;
    }

    public Modio(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.NoSharedPCHs;
        PrivatePCHHeaderFile = "Private/ModioPrivatePCH.h";
        bEnableUndefinedIdentifierWarnings = false;
#if UE_5_3_OR_LATER
	    IWYUSupport = IWYUSupport.Full;
	    CppStandard = CppStandardVersion.Cpp17;
#else
		bEnforceIWYU = true;
#endif
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

#if ENABLE_TRACE_LOG
        DumpNativePlatformConfig(PlatformConfig);
#endif

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

        // Apply internal testing config
        ApplyTestConfig(GeneratedHeaderPath, GeneratedSourcePath);

    }
}
