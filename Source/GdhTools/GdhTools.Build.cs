﻿// Copyright Ashot Barkhudaryan. All Rights Reserved.

using UnrealBuildTool;

public class GdhTools : ModuleRules
{
	public GdhTools(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PrivateDependencyModuleNames.AddRange(new[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"UnrealEd",
			"Slate",
			"SlateCore",
			"InputCore",
			"ToolMenus",
			"EditorStyle",
			"EditorScriptingUtilities",
			"DeveloperSettings",
			"GdhCore",
			"GdhEditor"
		});
	}
}