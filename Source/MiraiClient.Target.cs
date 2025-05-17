// Copyright CtrlCtrlV, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class MiraiClientTarget : TargetRules
{
	public MiraiClientTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Client;

		ExtraModuleNames.AddRange(new string[] { "MiraiGame" });

		MiraiGameTarget.ApplySharedMiraiTargetSettings(this);
	}
}
