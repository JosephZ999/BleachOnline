// Authors MoonDi & JosephZzz for Bleach Online fan game

#include "BOAbilityBase.h"
#include "BOAbilityTypes.h"
#include "BOCharacterBase.h"
#include "BOIndicatorComponent.h"
#include "TimerManager.h"

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

	bActive = true;
}

void UBOAbilityBase::Activate()
{
	if (bActive)
	{
		OnActivate(FAbilityParam());
		bActive = false;
		SetCooldownTimer();
	}
}

void UBOAbilityBase::ActivateWithParam(const FAbilityParam& Param)
{
	if (bActive)
	{
		OnActivate(Param);
		bActive = false;
		SetCooldownTimer();
	}
}

void UBOAbilityBase::SetCooldownTimer()
{
	if (Cooldown > 0.f)
	{
		FTimerHandle CooldownTimer;
		OwnerCharacter->GetWorldTimerManager().SetTimer(CooldownTimer, this, &ThisClass::OnCooldown, Cooldown);
		return;
	}
	OnCooldown();
}

void UBOAbilityBase::OnCooldown() 
{
	bActive = true;
}
