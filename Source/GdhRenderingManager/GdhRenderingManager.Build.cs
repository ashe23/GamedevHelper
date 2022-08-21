// Copyright Ashot Barkhudaryan. All Rights Reserved.

using UnrealBuildTool;

public class GdhRenderingManager : ModuleRules
{
	public GdhRenderingManager(ReadOnlyTargetRules Target) : base(Target)
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
				"PythonScriptPlugin",
				"DeveloperSettings",
				"PropertyEditor",
				"EditorStyle",
				"MovieScene",
				"LevelSequence",
				"InputCore",
				"UnrealEd",
				// MovieRender
				"MovieRenderPipelineCore",
				"MovieRenderPipelineRenderPasses",
				"MovieRenderPipelineSettings",
				"MovieRenderPipelineEditor",
				// Our modules
				"GdhCore", 
			}
		);
	}
}