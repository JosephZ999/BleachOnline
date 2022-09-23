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
	Owner		   = InOwnerChar;
	IndicatorType  = InIndicatorType;
	Consumption	   = InConsumption;
	Cooldown	   = InCooldown;
	ChargeNum	   = InChargesNum;
	CurrentCharges = ChargeNum;

	const bool bUseIndicator = IndicatorType != EIndicatorType::None;
	if (bUseIndicator)
	{
		IASCharacterInterface* CHI = Cast<IASCharacterInterface>(Owner);
		if (CHI)
		{
			Indicator = CHI->IGetIndicatorComponent(IndicatorType);
			if (Indicator)
			{
				checkf(Indicator && Cast<IASIndicatorInterface>(Indicator), TEXT("Indicator is not implements AbilitySystemIndicatorInterface"));
			}
			else
			{
				UE_LOG(LogAbility, Warning, TEXT("Indicator is null"));
			}
		}
	}
	checkf(Cast<IASCharacterInterface>(Owner), TEXT("Character is not implements AbilitySystemCharacterInterface"));

	bActive = true;
}

void UAbilityBase::Activate()
{
	if (bActive && IsEnoughtPower())
	{
		OnActivate();
		if (--CurrentCharges == 0)
		{
			SetCooldownTimer();
		}
	}
}

void UAbilityBase::ActivateWithParam(const FAbilityParam& Param)
{
	if (bActive && IsEnoughtPower())
	{
		OnActivateWithParam(Param);
		if (--CurrentCharges == 0)
		{
			SetCooldownTimer();
		}
	}
}

void UAbilityBase::SetCooldownTimer()
{
	bActive = false;
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
	CurrentCharges = ChargeNum;
}

bool UAbilityBase::IsEnoughtPower()
{
	auto IndicatorInterface = Cast<IASIndicatorInterface>(Indicator);
	if (! IndicatorInterface) return true;

	return IndicatorInterface->IGetValue() >= Consumption;
}

void UAbilityBase::OnActivate()
{
	auto IndicatorInterface = Cast<IASIndicatorInterface>(Indicator);
	if (! IndicatorInterface) return;

	IndicatorInterface->ISetValue(IndicatorInterface->IGetValue() - Consumption);
}

void UAbilityBase::OnActivateWithParam(const FAbilityParam& Param)
{
	auto IndicatorInterface = Cast<IASIndicatorInterface>(Indicator);
	if (! IndicatorInterface) return;

	IndicatorInterface->ISetValue(IndicatorInterface->IGetValue() - Consumption);
}
