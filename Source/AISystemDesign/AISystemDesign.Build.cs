// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AISystemDesign : ModuleRules
{
	public AISystemDesign(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput","AIModule", "GameplayTasks", "NavigationSystem", "UMG", "Slate", "SlateCore"});
	}
}
