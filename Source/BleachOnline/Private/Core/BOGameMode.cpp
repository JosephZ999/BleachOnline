// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "BOGameMode.h"
#include "BOGameState.h"
#include "BOPlayerController.h"
#include "BOPlayerState.h"
#include "TimerManager.h"

void ABOGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	UE_LOG(LogGameMode, Warning, TEXT("Init Game"));
}

void ABOGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	UE_LOG(LogGameMode, Warning, TEXT("PostLogin"));
}

void ABOGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}

void ABOGameMode::InitGameState()
{
	Super::InitGameState();
	
	if (! GetState()) return;

	for (auto PS : GetState()->PlayerArray)
	{
		auto PC = Cast<APlayerController>(PS->GetOwner());
		PC->StartSpot = nullptr;

		RestartPlayer(PC);

		auto BOPlayerState = Cast<ABOPlayerState>(PS);
		if (BOPlayerState)
		{
			BOPlayerState->ShowPlayerGameUI();
		}

		if (PC->IsLocalPlayerController())
		{
			
		}
	
	}
}

ABOGameState* ABOGameMode::GetState()
{
	if (State)
	{
		return State;
	}
	return State = Cast<ABOGameState>(GameState);
}

void ABOGameMode::SetStartMatchTimer(bool ForceStart)
{
	if (! GetWorld()) return;
	GetWorldTimerManager().SetTimer(StartMatchTimer, this, &ThisClass::StartMatchHandle, 1.f, true);
}

void ABOGameMode::StartMatchHandle()
{
	if (GetState() && GetState()->CanStartMatch())
	{
		// Start
	}
}
