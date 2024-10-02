// Copyright by person HDD  

using UnrealBuildTool;
using System.Collections.Generic;

public class DuraEditorTarget : TargetRules
{
	public DuraEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V4;

		ExtraModuleNames.AddRange( new string[] { "Dura" } );
	}
}
