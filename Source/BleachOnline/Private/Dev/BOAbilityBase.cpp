// Authors MoonDi & JosephZzz for Bleach Online fan game

#include "BOAbilityBase.h"
#include "BOCharacterBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogAbility, All, All);

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
	UE_LOG(LogAbility, Display, TEXT("Ability created and initialized successfully"));
}
