// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "BOPlayerState.h"
#include "BOGameState.h"

#include "BOPlayerController.h"
#include "BOGameInstance.h"
#include "UnrealNetwork.h"
#include "Engine\World.h"

DEFINE_LOG_CATEGORY_STATIC(LogPlayerState, All, All);

ABOPlayerState::ABOPlayerState()
{
	SetReplicates(true);
}

void ABOPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ABOPlayerState, Profile, COND_None);
}

void ABOPlayerState::BeginPlay()
{
	Super::BeginPlay();

	auto PC = Cast<ABOPlayerController>(GetOwner());
	if (! PC) return;

	if (PC->IsLocalController())
	{
		auto GI = GetGameInstance<UBOGameInstance>();
		if (GI)
		{
			SendPlayerProfileToServer(GI->GetPlayerProfile());
		}
	}
}

void ABOPlayerState::ChangePlayerState_Implementation(const FName& StateName)
{
	auto PC = Cast<ABOPlayerController>(GetOwner());
	if (PC)
	{
		PC->ChangeState(StateName);
	}
}

void ABOPlayerState::SendPlayerProfileToServer_Implementation(FPlayerProfile NewProfile)
{
	Profile = NewProfile;
	OnRep_PlayerProfile();
}

void ABOPlayerState::OnRep_PlayerProfile()
{
	if (! GetWorld()) return;

	auto GS = GetWorld()->GetGameState<ABOGameState>();
	if (! GS) return;

	GS->Join(this);
}
