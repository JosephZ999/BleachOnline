// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "BOPlayerState.h"
#include "BOPlayerController.h"
#include "BOGameInstance.h"
#include "UnrealNetwork.h"

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
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Error, TEXT("Server Profile Updating"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Client Profile Updating"));
	}
}
