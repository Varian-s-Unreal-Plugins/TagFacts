// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;       // Required for Directory and Path

public class TagFacts : ModuleRules
{
	public TagFacts(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		//Path to the current plugin directory
		string PluginPath = Path.GetFullPath(Path.Combine(ModuleDirectory, "../../../"));

		//Check if the Cog plugin exists
		if (Directory.Exists(Path.Combine(PluginPath, "Cog")))
		{
			PublicDefinitions.Add("COG_INSTALLED=1");
			PublicDependencyModuleNames.Add("CogCommon");
			// Other Cog plugins can be added only for specific target configuration
			if (Target.Configuration != UnrealTargetConfiguration.Shipping)
			{
				PublicDependencyModuleNames.AddRange(new string[]
				{
					"CogDebug",
					"CogEngine",
					"CogImgui",
					"CogWindow",
				});
			}
		}
		else
		{
			PublicDefinitions.Add("COG_INSTALLED=0");
		}
		
		//Check if the Hermes plugin exists
		if (Directory.Exists(Path.Combine(PluginPath, "HermesCommunications")))
		{
			PublicDefinitions.Add("HERMES_INSTALLED=1");
			PublicDependencyModuleNames.Add("HermesCommunications");
		}
		else
		{
			PublicDefinitions.Add("HERMES_INSTALLED=0");
		}
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"GameplayTags",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore"
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
