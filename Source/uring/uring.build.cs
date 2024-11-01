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
using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;

#if UE_5_0_OR_LATER
using EpicGames.Core;
#else
using Tools.DotNETCommon;
#endif

public class uring : ModuleRules
{
    private bool bDevelopmentModeOverride = false;
    private bool IsDevelopmentMode()
    {
        return bDevelopmentModeOverride || File.Exists(Path.Combine(PluginDirectory, "../../.modio_development_mode"));
    }
    private void ApplyProjectDefinitions(ReadOnlyTargetRules Target)
    {
        if (Target.ProjectDefinitions.Contains("MODIO_DEVELOPMENT_MODE"))
        {
            bDevelopmentModeOverride = true;
        }
        else if (System.Environment.GetEnvironmentVariables().Contains("MODIO_DEVELOPMENT_MODE"))
        {
            bDevelopmentModeOverride = true;
        }
    }

    public uring(ReadOnlyTargetRules Target) : base(Target)
    {
        ApplyProjectDefinitions(Target);

        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        string GeneratedSourcePath = Path.Combine(ModuleDirectory, "../ThirdParty", "GeneratedUringSource");

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine" });
        PublicIncludePaths.AddRange(new string[] { Path.Combine(ModuleDirectory, "Public"), Path.Combine(ModuleDirectory, "../ThirdParty/liburing/src/include") });
        PrivateIncludePaths.AddRange(new string[] { Path.Combine(GeneratedSourcePath), Path.Combine(ModuleDirectory, "../ThirdParty/liburing/src") });
        PrivateDefinitions.Add("LIBURING_INTERNAL=1");

        if (IsDevelopmentMode())
        {

            string OldGeneratedSourcePath = Path.Combine(ModuleDirectory, "Private", "GeneratedSource");
            // Delete the old Generated Source path if its present, as it may conflict with the new way we add GeneratedSource for this module
            if (Directory.Exists(OldGeneratedSourcePath))
            {
                Directory.Delete(OldGeneratedSourcePath, true);
            }

            // Set up the new GeneratedSourcePath. This folder sits outside the module directory so we can use
            // ConditionalAddModuleDirectory. Relying on UBT to pull in files that we have copied via this script
            // is unreliable and causes intermittent build issues.
            if (Directory.Exists(GeneratedSourcePath))
            {
                Directory.Delete(GeneratedSourcePath, true);
            }

            Directory.CreateDirectory(GeneratedSourcePath);

            List<string> CFiles = new List<string>(Directory.GetFiles(Path.Combine(ModuleDirectory, "../ThirdParty/liburing/src"), "*.c", SearchOption.AllDirectories));

            foreach (string CFile in CFiles)
            {
                //Add the original file in our upstream repository as a dependency, so if a user edits it we will copy it over
                ExternalDependencies.Add(CFile);
                string DestinationPath = Path.Combine(GeneratedSourcePath, Path.GetFileName(CFile));
                File.Copy(CFile, DestinationPath, true);
            }

            if (File.Exists(Path.Combine(GeneratedSourcePath, "nolibc.c")))
            {
                File.Delete(Path.Combine(GeneratedSourcePath, "nolibc.c"));
            }

            

            {
                string CompatHeader = @"/* SPDX-License-Identifier: MIT */
#ifndef LIBURING_COMPAT_H
#define LIBURING_COMPAT_H

#include <stdint.h>
#include <endian.h>

struct __kernel_timespec
    {
        int64_t tv_sec;
        long long tv_nsec;
    };

# include <inttypes.h>

    struct open_how
    {
        uint64_t flags;
        uint64_t mode;
        uint64_t resolve;
    };

#ifndef __kernel_rwf_t
    typedef int __kernel_rwf_t;
#endif

#endif
";
                string CompatHeaderPath = Path.Combine(ModuleDirectory, "Public", "liburing");
                Directory.CreateDirectory(CompatHeaderPath);
                File.WriteAllText(Path.Combine(CompatHeaderPath, "compat.h"), CompatHeader);
            }
        }

        ConditionalAddModuleDirectory(new DirectoryReference(GeneratedSourcePath));
    }
}