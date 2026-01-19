// Copyright (c) 2025, dvolkov. All rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;

[SupportedPlatforms(UnrealPlatformClass.Server)]
public class MiraiServerTarget : TargetRules
{
	public MiraiServerTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Server;

		ExtraModuleNames.AddRange(new string[] { "MiraiGame" });

		MiraiGameTarget.ApplySharedMiraiTargetSettings(this);

		bUseChecksInShipping = true;
	}
}
