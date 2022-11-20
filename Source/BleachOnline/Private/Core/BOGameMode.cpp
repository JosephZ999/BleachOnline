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

	// ResetPlayerUI(NewPlayer);
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

void ABOGameMode::RestartPlayer(AController* NewPlayer)
{
	Super::RestartPlayer(NewPlayer);
	ResetPlayerUI(NewPlayer);
}

ABOGameState* ABOGameMode::GetState()
{
	if (State)
	{
		return State;
	}
	return State = Cast<ABOGameState>(GameState);
}

void ABOGameMode::SetGameSetting(AController* Player, const FGameSettingsParam& NewGameSetting)
{
	if (! GetState()) return;

	if (! GetState()->IsAdmin(Player->GetPlayerState<ABOPlayerState>())) return;

	for (auto& Param : GameSettings)
	{
		if (Param.Index != NewGameSetting.Index) continue;

		Param = NewGameSetting;
	}
}

void ABOGameMode::ApplyGameSettings()
{
	for (auto& Param : GameSettings)
	{
		switch (Param.Index)
		{	// clang-format off
		case EGameSettingsIndex::GameTime_I:    break;
		case EGameSettingsIndex::FrendlyFire_B: break;
			// clang-format on
		}	// switch end
	}		// loop end
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
	if (auto PC = Cast<ABOPlayerController>(Player))
	{
		PC->HideAllWidgets();

		if (GetState()->IsAdmin(PC->GetPlayerState<ABOPlayerState>()))
		{
			PC->ShowPlayerGameSettings();
		}
		else
		{
			PC->ShowPlayerGameUI();
		}
	}
}
