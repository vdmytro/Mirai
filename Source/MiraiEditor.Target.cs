// Copyright CtrlCtrlV, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class MiraiEditorTarget : TargetRules
{
	public MiraiEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange(new string[] { "MiraiGame", "MiraiEditor" });

		if (!bBuildAllModules)
		{
			NativePointerMemberBehaviorOverride = PointerMemberBehavior.Disallow;
		}
		
		MiraiGameTarget.ApplySharedMiraiTargetSettings(this);
	}
}
