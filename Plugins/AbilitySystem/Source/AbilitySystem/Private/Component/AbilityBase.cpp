// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "AbilityBase.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"

#include "ASCharacterInterface.h"
#include "ASIndicatorInterface.h"

DEFINE_LOG_CATEGORY_STATIC(LogAbility, All, All);

void UAbilityBase::Initialize(		//
	AActor*		   InOwnerChar,		//
	EIndicatorType InIndicatorType, //
	float		   InConsumption,	//
	float		   InCooldown,		//
	uint8		   InChargesNum)
{
	Owner		  = InOwnerChar;
	IndicatorType = InIndicatorType;
	Consumption	  = InConsumption;
	Cooldown	  = InCooldown;
	ChargesNum	  = InChargesNum;

	const bool bUseIndicator = IndicatorType != EIndicatorType::None;
	if (bUseIndicator)
	{
		IASCharacterInterface* CHI = Cast<IASCharacterInterface>(Owner);
		if (CHI)
		{
			Indicator = CHI->IGetIndicator(IndicatorType);
		}
		if (! Indicator)
		{
			UE_LOG(LogAbility, Error, TEXT("Cannot find Character indicator"));
		}
	}

	checkf(Owner, TEXT("Character is null"));
	UE_LOG(LogAbility, Display, TEXT("Ability created and initialized successfully"));

	bActive = true;
}

void UAbilityBase::Activate()
{
	if (bActive && IsEnoughtPower())
	{
		OnActivate();
		bActive = false;
		SetCooldownTimer();
	}
}

void UAbilityBase::ActivateWithParam(const FAbilityParam& Param)
{
	if (bActive && IsEnoughtPower())
	{
		OnActivateWithParam(Param);
		bActive = false;
		SetCooldownTimer();
	}
}

void UAbilityBase::SetCooldownTimer()
{
	if (Cooldown > 0.f)
	{
		FTimerHandle CooldownTimer;
		Owner->GetWorldTimerManager().SetTimer(CooldownTimer, this, &ThisClass::OnCooldown, Cooldown);
		return;
	}
	OnCooldown();
}

void UAbilityBase::OnCooldown()
{
	bActive = true;
}

bool UAbilityBase::IsEnoughtPower()
{
	auto IndicatorInterface = Cast<IASIndicatorInterface>(Indicator);
	if (!IndicatorInterface) return true;

	return IndicatorInterface->IGetValue() >= Consumption;
}

void UAbilityBase::OnActivate()
{
	auto IndicatorInterface = Cast<IASIndicatorInterface>(Indicator);
	if (! IndicatorInterface) return;

	IndicatorInterface->ISetValue(IndicatorInterface->IGetValue() - Consumption);
	UE_LOG(LogAbility, Warning, TEXT("--Power"));

}

void UAbilityBase::OnActivateWithParam(const FAbilityParam& Param) 
{
	auto IndicatorInterface = Cast<IASIndicatorInterface>(Indicator);
	if (!IndicatorInterface) return;

	IndicatorInterface->ISetValue(IndicatorInterface->IGetValue() - Consumption);
	UE_LOG(LogAbility, Warning, TEXT("--Power"));
}
