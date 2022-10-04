// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "BOGameState.h"
#include "BOHUD.h"
#include "BOPlayerController.h"

ABOGameState::ABOGameState()
{
	bReplicates = true;
}

bool ABOGameState::CanStartMatch()
{
	// check for players ready
	return false;
}