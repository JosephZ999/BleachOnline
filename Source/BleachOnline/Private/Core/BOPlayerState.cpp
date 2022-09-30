// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "BOPlayerState.h"
#include "BOPlayerController.h"

ABOPlayerState::ABOPlayerState()
{
	SetReplicates(true);
}

void ABOPlayerState::ChangePlayerState_Implementation(const FName& StateName)
{
	auto PC = Cast<ABOPlayerController>(GetOwner());
	if (PC)
	{
		PC->ChangeState(StateName);
	}
	else
	{

		UE_LOG(LogTemp, Warning, TEXT("AAAAAAAAAAAAAAAAAAA"));
	}
}
