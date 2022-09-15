// Authors MoonDi & JosephZzz for Bleach Online fan game

using UnrealBuildTool;
using System.Collections.Generic;

public class BleachOnlineTarget : TargetRules
{
	public BleachOnlineTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "BleachOnline" } );
	}
}
