// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Project_X : ModuleRules
{
	public Project_X(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput","SlateCore","Slate","UMG" });
	}
}
