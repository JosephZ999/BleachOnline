// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "BOPlayerListComponent.h"
#include "BOPlayerState.h"

UBOPlayerListComponent::UBOPlayerListComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UBOPlayerListComponent::SetPlayerList(PlayerList* PlayerArray)
{
	List = PlayerArray;
}

int32 UBOPlayerListComponent::GetPlayersNum() const
{
	return List->Num();
}

APlayerState* UBOPlayerListComponent::GetLocalPlayer()
{
	if (LocalPlayer) return LocalPlayer;

	for (APlayerState* Player : *List)
	{
		if (Player->bIsABot) continue;

		return LocalPlayer = Player;
	}
	return nullptr;
}
