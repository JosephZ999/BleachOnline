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

void ABOGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

	UE_LOG(LogGameMode, Warning, TEXT("Pre Login /------------------------/"));
	UE_LOG(LogGameMode, Warning, TEXT("Options - %s"), *Options);
	UE_LOG(LogGameMode, Warning, TEXT("Address - %s"), *Address);
	UE_LOG(LogGameMode, Warning, TEXT("ErrorMessage - %s"), *ErrorMessage);
}

void ABOGameMode::PostLogin(APlayerController* NewPlayer)
{
	/*if (GetState() && ! GetState()->GetAdminPlayer())
	{
		GetState()->SetAdminPlayer(NewPlayer->GetPlayerState<ABOPlayerState>());
	}*/

	auto PC = Cast<ABOPlayerController>(NewPlayer);
	if (PC)
	{
		PC->GetPlayerProfile();
	}
	Super::PostLogin(NewPlayer);
}

void ABOGameMode::Logout(AController* Exiting)
{
	if (! GetState()) return;

	auto PlayerState = Exiting->GetPlayerState<ABOPlayerState>();
	GetState()->RemovePlayer(PlayerState->GetId());

	if (PlayerState && GetState()->IsAdmin(PlayerState))
	{
		UE_LOG(LogGameMode, Warning, TEXT("Admin has left the game"));
	}

	Super::Logout(Exiting);
}

void ABOGameMode::InitGameState()
{
	Super::InitGameState();
}

void ABOGameMode::ResetLevel()
{
	Super::ResetLevel();

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

void ABOGameMode::SetGameSetting(AController* Player, const FGameParam& NewGameSetting)
{
	if (! GetState()) return;

	if (! GetState()->IsAdmin(Player->GetPlayerState<ABOPlayerState>())) return;

	for (auto& Param : GameSettings.Params)
	{
		if (Param.Index != NewGameSetting.Index) continue;

		Param = NewGameSetting;
	}
}

void ABOGameMode::SendPlayerProfile(AController* Player, FPlayerProfile Profile)
{
	if (true) // Ignore if you want
	{
		OnGetPlayerProfile(Player, Profile);
	}
}

void ABOGameMode::ApplyGameSettings()
{
	for (auto& Param : GameSettings.Params)
	{
		switch (Param.Index)
		{	// clang-format off
		case EGameParamIndex::GameTime_I:    break;
		case EGameParamIndex::FrendlyFire_B: break;
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

void ABOGameMode::OnGetPlayerProfile(AController* Player, FPlayerProfile& Profile)
{
	auto GS = Cast<ABOGameState>(GameState);
	if (GS)
	{
		GS->AddPlayer(Player, Profile);
	}
}
