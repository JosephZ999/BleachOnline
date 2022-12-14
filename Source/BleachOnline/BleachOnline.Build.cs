// Authors MoonDi & JosephZzz for Bleach Online fan game

using UnrealBuildTool;

public class BleachOnline : ModuleRules
{
	public BleachOnline(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] 
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "Paper2D",
            "AbilitySystem",
            "ImageDownloadManager",
            "ImageWrapper",
            "DesktopPlatform",
        });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        PublicIncludePaths.AddRange(new string[] 
        {
            "BleachOnline/Public/CharacterComponents",
            "BleachOnline/Public/Chars",
            "BleachOnline/Public/Chars/Heroes",
            "BleachOnline/Public/Dev",
            "BleachOnline/Public/Core",
            "BleachOnline/Public/Core/Components",
            "BleachOnline/Public/Core/Subsystem",
            "BleachOnline/Public/UI",
            "BleachOnline/Public/Utils",
            "BleachOnline/Public/Pickups",
            "BleachOnline/Public/AI",

        });

        // Uncomment if you are using Slate UI
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
