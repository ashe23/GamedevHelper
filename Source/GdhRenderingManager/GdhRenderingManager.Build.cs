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
				// MovieRender
				"MovieRenderPipelineCore",
				"MovieRenderPipelineRenderPasses",
				"MovieRenderPipelineSettings",
				// Our modules
				"GdhCore", 
			}
		);
	}
}