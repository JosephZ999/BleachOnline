// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "BOGameMode.h"
#include "BOGameState.h"
#include "BOPlayerController.h"
#include "BOPlayerState.h"

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
	for (auto PS : GetState()->PlayerArray)
	{
		Cast<AController>(PS->GetOwner())->StartSpot = nullptr;
		RestartPlayer(Cast<AController>(PS->GetOwner()));
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
