// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FreeflowCombat : ModuleRules
{
	public FreeflowCombat(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "MotionWarping" });
	}
}
