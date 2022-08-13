// Copyright Ashot Barkhudaryan. All Rights Reserved.

using UnrealBuildTool;

public class GamedevHelper : ModuleRules
{
	public GamedevHelper(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new []
			{
				"Core",
			}
		);


		PrivateDependencyModuleNames.AddRange(
			new []
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
				"Projects",
				"ToolMenus",
				"EditorStyle",
				"InputCore",
				"MediaAssets",
				"LevelSequence",
				"Foliage",
				"UMG",
				"UMGEditor",
				"AIModule",
				"PhysicsCore",
				"Landscape",
				"Niagara",
				"NiagaraEditor",
				"EditorScriptingUtilities",
				"Blutility",
				"Paper2D",
				"IntroTutorials",
				"CinematicCamera",
				"Settings",
				"MovieRenderPipelineCore",
				"MovieRenderPipelineEditor",
				"MovieRenderPipelineRenderPasses",
				"MovieRenderPipelineEditor",
				"MovieRenderPipelineSettings",
				"LevelSequence",
				"MovieScene",
				"SequencerScripting",
				"EditorSubsystem",
				"PythonScriptPlugin",
				"Json"
			}
		);
	}
}