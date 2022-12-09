// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "BOPlayerListComponent.h"
#include "BOPlayerState.h"
#include "GameFramework\Controller.h"

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
		if (IsLocalPlayer(Player))
		{
			return LocalPlayer = Player;
		}
	}
	return nullptr;
}

TArray<APlayerState*> UBOPlayerListComponent::GetAllPlayers() const
{
	return *List;
}

TArray<APlayerState*> UBOPlayerListComponent::GetOtherPlayers() const
{
	TArray<APlayerState*> NewList;
	for (auto Player : *List)
	{
		if (IsLocalPlayer(Player)) continue;

		NewList.Add(Player);
	}
	return NewList;
}

FPlayerProfile UBOPlayerListComponent::GetPlayerProfile(const APlayerState* Player) const
{
	auto PS = Cast<ABOPlayerState>(Player);
	if (PS)
	{
		return PS->GetProfile();
	}
	return FPlayerProfile();
}

bool UBOPlayerListComponent::IsLocalPlayer(const APlayerState* Player) const
{
	auto PC = Cast<AController>(Player->GetOwner());
	return PC && PC->IsLocalController();
}
