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
				"DeveloperSettings",
				"Projects",
				"UnrealEd",
				"LevelSequence",
				"MovieScene",
				"LevelEditor",
				"EditorSubsystem",
				"EditorStyle",
				"InputCore",
				"Json",
				"PythonScriptPlugin",
				// MovieRender
				"MovieRenderPipelineCore",
				"MovieRenderPipelineRenderPasses",
				"MovieRenderPipelineSettings",
				"MovieRenderPipelineEditor",
				"MovieSceneTracks",
				"MovieScene",
			}
		);
	}
}