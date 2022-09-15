// Authors MoonDi & JosephZzz for Bleach Online fan game

using UnrealBuildTool;
using System.Collections.Generic;

public class BleachOnlineEditorTarget : TargetRules
{
	public BleachOnlineEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "BleachOnline" } );
	}
}
