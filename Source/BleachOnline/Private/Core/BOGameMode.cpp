// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "BOGameMode.h"
#include "BOGameState.h"
#include "BOPlayerController.h"
#include "BOPlayerState.h"
#include "TimerManager.h"

void ABOGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}

void ABOGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	if (GetState() && ! GetState()->GetAdminPlayer())
	{
		GetState()->SetAdminPlayer(NewPlayer->GetPlayerState<ABOPlayerState>());
	}
}

void ABOGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	if (! GetState()) return;

	auto PlayerState = Exiting->GetPlayerState<ABOPlayerState>();
	if (PlayerState && GetState()->IsAdmin(PlayerState))
	{
		UE_LOG(LogGameMode, Warning, TEXT("Admin has left the game"));
	}
}

void ABOGameMode::InitGameState()
{
	Super::InitGameState();

	if (! GetState()) return;

	for (auto PlayerState : GetState()->PlayerArray)
	{
		auto PC = Cast<AController>(PlayerState->GetOwner());
		if (! PC) continue;

		auto PS = Cast<ABOPlayerState>(PlayerState);
		if (! PS) continue;

		// Respawn
		PC->StartSpot = nullptr;
		RestartPlayer(PC);

		PS->HideAllWidgets();
		PS->ShowPlayerGameUI();

		if (GetState()->IsAdmin(PS))
		{
			PS->ShowPlayerGameSettings();
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

void ABOGameMode::SetGameSetting(ABOPlayerState* Player, const FGameSettings& NewGameSetting)
{
	if (GetState() && GetState()->IsAdmin(Player))
	{
		Settings = NewGameSetting;
	}
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
