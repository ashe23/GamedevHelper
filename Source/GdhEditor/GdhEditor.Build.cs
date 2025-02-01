// Copyright Ashot Barkhudaryan. All Rights Reserved.

using UnrealBuildTool;

public class GdhEditor : ModuleRules
{
	public GdhEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PrivateDependencyModuleNames.AddRange(new[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"UnrealEd",
			"EditorStyle",
            "EditorWidgets",
            "EditorScriptingUtilities",
			"Slate",
			"SlateCore",
			"Projects",
			"GdhCore",
            "MovieRenderPipelineCore",
            "MovieRenderPipelineEditor",
            "MovieRenderPipelineRenderPasses",
            "MovieRenderPipelineSettings",
            "LevelSequence",
            "MovieScene",
            "MovieSceneTracks",
            "InputCore",
            "ToolMenus",
            "DeveloperSettings",
		});
	}
}