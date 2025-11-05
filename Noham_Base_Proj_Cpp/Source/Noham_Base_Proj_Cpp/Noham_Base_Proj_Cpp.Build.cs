// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Noham_Base_Proj_Cpp : ModuleRules
{
	public Noham_Base_Proj_Cpp(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"UMG",
			"NEON",
			"Json",
			"JsonUtilities",
			"RHI",
			"RenderCore"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {
			"Slate",
			"SlateCore"
		});

		// NEON Plugin Support (when available)
		if (Target.bBuildEditor)
		{
			PrivateDependencyModuleNames.AddRange(new string[] {
				"UnrealEd",
				"ToolMenus",
				"EditorStyle",
				"EditorWidgets"
			});
		}
	}
}
