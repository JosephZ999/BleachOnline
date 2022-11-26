// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "BOPlayerState.h"
#include "BOPlayerController.h"

DEFINE_LOG_CATEGORY_STATIC(LogPlayerState, All, All);

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
}

void ABOPlayerState::AddedANewPlayer_Implementation(FPlayerGameProfile NewPlayer)
{
	// ...
}

void ABOPlayerState::SetId(int32 NewId)
{
	PlayerId = NewId;
}
