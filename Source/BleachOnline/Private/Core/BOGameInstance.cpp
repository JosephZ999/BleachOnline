// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "BOGameInstance.h"

UBOGameInstance::UBOGameInstance()
{
	FString NameString = "Player " + FString::FromInt(FMath::RandRange(0, 9));
	Profile.Name = FText::FromString(NameString);
}

FPlayerProfile& UBOGameInstance::GetPlayerProfile()
{
	return Profile;
}