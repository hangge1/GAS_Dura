// Copyright by person HDD  

using UnrealBuildTool;
using System.Collections.Generic;

public class DuraTarget : TargetRules
{
	public DuraTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;

		ExtraModuleNames.AddRange( new string[] { "Dura" } );
	}
}
