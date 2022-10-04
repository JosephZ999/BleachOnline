// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "BOPlayerController.h"

ABOPlayerController::ABOPlayerController()
{
	bShowMouseCursor = true;
}

void ABOPlayerController::Reset()
{
	ChangeState(NAME_Playing);
}
