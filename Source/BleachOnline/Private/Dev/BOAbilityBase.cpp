// Authors MoonDi & JosephZzz for Bleach Online fan game

#include "BOAbilityBase.h"
#include "BOCharacterBase.h"
#include "BOIndicatorComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogAbility, All, All);

void UBOAbilityBase::Initialize(	   //
	ABOCharacterBase* InOwnerChar,	   //
	EIndicatorType	  InIndicatorType, //
	float			  InConsumption,   //
	float			  InCooldown,	   //
	uint8			  InChargesNum)
{
	OwnerCharacter = InOwnerChar;
	IndicatorType  = InIndicatorType;
	Consumption	   = InConsumption;
	Cooldown	   = InCooldown;
	ChargesNum	   = InChargesNum;
	bUseIndicator  = IndicatorType != EIndicatorType::None;

	if (bUseIndicator)
	{
		CharacterIndicator = OwnerCharacter->GetIndicator(IndicatorType);
		UE_LOG(LogAbility, Error, TEXT("Cannot find Character indicator"));
	}

	checkf(OwnerCharacter, TEXT("Character is null"));
	UE_LOG(LogAbility, Display, TEXT("Ability created and initialized successfully"));
}
