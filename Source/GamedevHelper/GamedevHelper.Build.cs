// Copyright Ashot Barkhudaryan. All Rights Reserved.

using UnrealBuildTool;

public class GamedevHelper : ModuleRules
{
	public GamedevHelper(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
			}
		);


		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"NetCore",
				"AssetRegistry",
				"AssetTools",
				"ContentBrowser",
				"UnrealEd",
				"RenderCore",
				"EditorScriptingUtilities",
			}
		);
	}
}