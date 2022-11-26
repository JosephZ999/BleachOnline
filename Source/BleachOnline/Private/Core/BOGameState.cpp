// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "BOGameState.h"
#include "BOHUD.h"
#include "BOPlayerController.h"
#include "BOPlayerState.h"

ABOGameState::ABOGameState()
{
	bReplicates = true;
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

void ABOGameState::AddPlayer(const AController* Player, const FPlayerProfile& Profile)
{
	// LastPlayerIndex

	++LastPlayerIndex;

	auto NewPlayerState = Player->GetPlayerState<ABOPlayerState>();
	NewPlayerState->SetId(LastPlayerIndex);

	auto NewPlayer = FPlayerGameProfile(LastPlayerIndex, Profile);
	ActivePlayers.Add(LastPlayerIndex, NewPlayer);

	// Send profil to other players
	AddedANewPlayer(NewPlayer);
	AddedANewPlayer_Implementation(NewPlayer); // Run on Server
}

void ABOGameState::RemovePlayer(int32 PlayerId)
{
	if (ActivePlayers.Contains(PlayerId))
	{
		ActivePlayers.Remove(PlayerId);
	}
}

void ABOGameState::AddedANewPlayer_Implementation(FPlayerGameProfile NewPlayer)
{
	// Client server
}
