// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Viltek : ModuleRules
{
	public Viltek(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] 
		{
			"Core", 
			"CoreUObject",
			"Engine",
			"InputCore", 
			"GeometryFramework", 
			"GeometryScriptingCore", 
			"MeshConversion", 
			"GeometryCore", 
			"MeshDescription", 
			"StaticMeshDescription",
			"SkeletalMeshDescription"
		});

		//PublicDependencyModuleNames.Add("GeometryFramework");
		//PublicDependencyModuleNames.Add("GeometryScriptingCore");

		// Uncomment if you are using Slate UI
		PrivateDependencyModuleNames.AddRange(new string[] 
		{ 
			"Slate",
			"SlateCore",
			"MeshUtilitiesCommon"
		});

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
