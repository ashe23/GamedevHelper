// Copyright Ashot Barkhudaryan. All Rights Reserved.

using UnrealBuildTool;

public class GdhUtil : ModuleRules
{
	public GdhUtil(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new[]
		{
			"GdhCore"
		});

		PrivateDependencyModuleNames.AddRange(new[]
		{
			"Core",
			"CoreUObject",
			"Engine"
		});
	}
}