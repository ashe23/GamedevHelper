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
				"Core"
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
				"AssetRegistry",
				"EditorSubsystem",
				"EditorStyle",
				"InputCore",
				"DeveloperSettings",
				"UMGEditor",
				"Blutility",
				"Foliage",
				"Niagara",
				"NiagaraEditor",
				"MediaAssets",
				"LevelSequence",
				"CinematicCamera",
				"AIModule",
				"Landscape",
				"PhysicsCore",
				"UMG",
				"IntroTutorials",
				"AssetTools",
				"ContentBrowser", 
				"EditorScriptingUtilities",
			}
		);
	}
}