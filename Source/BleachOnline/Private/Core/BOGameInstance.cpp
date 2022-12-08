// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "BOGameInstance.h"

UBOGameInstance::UBOGameInstance()
{
	Profile.Name = FText::FromString("RandomPlayer");
}

FPlayerProfile& UBOGameInstance::GetPlayerProfile()
{
	return Profile;
}