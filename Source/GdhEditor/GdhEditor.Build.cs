// Copyright Ashot Barkhudaryan. All Rights Reserved.

using UnrealBuildTool;

public class GdhEditor : ModuleRules
{
	public GdhEditor(ReadOnlyTargetRules Target) : base(Target)
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
				"Projects",
				"UnrealEd",
				"LevelSequence",
				"LevelEditor",
				"EditorSubsystem",
				// MovieRender
				"MovieRenderPipelineCore",
				"MovieRenderPipelineEditor",
				// our modules,
				"GdhCore",
				"GdhRenderingManager", 
			}
		);
	}
}