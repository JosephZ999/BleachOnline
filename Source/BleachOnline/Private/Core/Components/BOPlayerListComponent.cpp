// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "BOPlayerListComponent.h"
#include "BOPlayerState.h"
#include "GameFramework\Controller.h"

UBOPlayerListComponent::UBOPlayerListComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UBOPlayerListComponent::SetPlayerList(PlayerArray* PlayerArray)
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

PlayerArray UBOPlayerListComponent::GetAllPlayers() const
{
	return *List;
}

PlayerArray UBOPlayerListComponent::GetOtherPlayers() const
{
	PlayerArray NewList;
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
