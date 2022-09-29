// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "BOGameModeBase.h"
#include "BOPlayerControllerBase.h"
#include "BOSpectatorCharacter.h"

ABOGameModeBase::ABOGameModeBase()
{
	PlayerControllerClass = ABOPlayerControllerBase::StaticClass();
	DefaultPawnClass	  = ABOSpectatorCharacter::StaticClass();
}
