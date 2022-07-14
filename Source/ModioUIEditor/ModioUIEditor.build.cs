using UnrealBuildTool;
using System.IO;
using System.Collections.Generic;

public class ModioUIEditor : ModuleRules
{
    public ModioUIEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        bEnforceIWYU = true;
        
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "UnrealEd", "Modio", "ModioUI", "DetailCustomizations", "PropertyEditor", "PropertyPath" });

		PrivateDependencyModuleNames.AddRange(new string[] {"UMG", "Slate", "SlateCore","UnrealEd","AssetTools","AssetRegistry", "AppFramework", "InputCore"});

        PublicIncludePaths.AddRange(new string[] { Path.Combine(ModuleDirectory, "Public") });
        PrivateIncludePaths.AddRange(new string[] { Path.Combine(ModuleDirectory, "Private")
        });
        
    }
}