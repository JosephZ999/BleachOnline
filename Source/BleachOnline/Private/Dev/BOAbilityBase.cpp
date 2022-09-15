// Authors MoonDi & JosephZzz for Bleach Online fan game

#include "BOAbilityBase.h"
#include "BOCharacterBase.h"

void UBOAbilityBase::Initialize(	   //
	ABOCharacterBase* InOwnerChar,	   //
	EConsumptionType  InIndicatorType, //
	float			  InConsumption,   //
	float			  InCooldown,	   //
	uint8			  InChargesNum)
{
	OwnerCharacter = InOwnerChar;
	IndicatorType  = InIndicatorType;
	Consumption	   = InConsumption;
	Cooldown	   = InCooldown;
	ChargesNum	   = InChargesNum;

	checkf(OwnerCharacter, TEXT("Character is null"));
}
