// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "BOGameState.h"
#include "BOPlayerListComponent.h"

#include "BOHUD.h"
#include "BOPlayerController.h"
#include "BOPlayerState.h"

ABOGameState::ABOGameState()
{
	bReplicates = true;

	PlayerListComponent = CreateDefaultSubobject<UBOPlayerListComponent>("PlayerListComponent");
	PlayerListComponent->SetPlayerList(&PlayerArray);
}

bool ABOGameState::CanStartMatch()
{
	// check for players ready
	return false;
}

ABOPlayerState* ABOGameState::GetAdminPlayer()
{
	return AdminPlayer;
}

void ABOGameState::SetAdminPlayer(ABOPlayerState* inPlayer)
{
	AdminPlayer = inPlayer;
}

bool ABOGameState::IsAdmin(ABOPlayerState* InPlayer)
{
	return AdminPlayer == InPlayer;
}