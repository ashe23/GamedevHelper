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
				// MovieRender
				"MovieRenderPipelineCore",
				"MovieRenderPipelineRenderPasses",
				// Our modules
				"GdhCore", 
			}
		);
	}
}