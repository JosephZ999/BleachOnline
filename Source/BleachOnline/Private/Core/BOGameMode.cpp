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

	ResetPlayerUI(NewPlayer);
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

		// Respawn
		PC->StartSpot = nullptr;
		RestartPlayer(PC);
	}
}

void ABOGameMode::ResetLevel()
{
	Super::ResetLevel();

	ResetAllPlayersUI();
}

ABOGameState* ABOGameMode::GetState()
{
	if (State)
	{
		return State;
	}
	return State = Cast<ABOGameState>(GameState);
}

void ABOGameMode::SetGameSetting(AController* Player, const FGameSettings& NewGameSetting)
{
	auto PS = Player->GetPlayerState<ABOPlayerState>();
	if (GetState() && GetState()->IsAdmin(PS))
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

void ABOGameMode::ResetPlayerUI(AController* Player) 
{
	if (auto PS = Player->GetPlayerState<ABOPlayerState>())
	{
		PS->HideAllWidgets();

		if (GetState()->IsAdmin(PS))
		{
			PS->ShowPlayerGameSettings();
		}
		else
		{
			PS->ShowPlayerGameUI();
		}
	}
}

void ABOGameMode::ResetAllPlayersUI()
{
	for (auto PlayerState : GetState()->PlayerArray)
	{
		auto PC = Cast<AController>(PlayerState->GetOwner());
		if (!PC) continue;

		ResetPlayerUI(PC);
	}
}