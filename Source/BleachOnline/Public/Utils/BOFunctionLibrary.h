// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"

class UObject;
class UBOGameInstanceSubsystem;

struct BOGetterLib
{
	static UBOGameInstanceSubsystem* GetGameInstanceSubsystem(UObject* WorldContextObj);
};