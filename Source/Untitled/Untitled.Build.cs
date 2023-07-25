// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Untitled : ModuleRules
{
	public Untitled(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] 
		{ "Core", "CoreUObject", "Engine", "InputCore", "UMG", "NavigationSystem", "AIModule", "GameplayTasks",

            "HeadMountedDisplay", "EnhancedInput" });

		PrivateDependencyModuleNames.AddRange(new string[]
		{ "UntitledSub"
			});

    }
}
