// Copyright Ashot Barkhudaryan. All Rights Reserved.

using UnrealBuildTool;

public class Gdh : ModuleRules
{
	public Gdh(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new[]
			{
				"Core",
			}
		);


		PrivateDependencyModuleNames.AddRange(
			new[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"UnrealEd",
				"Projects",
				// "NetCore",
				// "AssetRegistry",
				// "AssetTools",
				// "ContentBrowser",
				// "RenderCore",
				// "EditorScriptingUtilities",
				// "ToolMenus",
				// "EditorStyle",
				// "InputCore",
				// "MediaAssets",
				// "LevelSequence",
				// "Foliage",
				// "UMG",
				// "UMGEditor",
				// "AIModule",
				// "PhysicsCore",
				// "Landscape",
				// "Niagara",
				// "NiagaraEditor",
				// "EditorScriptingUtilities",
				// "Blutility",
				// "Paper2D",
				// "IntroTutorials",
				// "CinematicCamera",
				// "Settings"
			}
		);
	}
}