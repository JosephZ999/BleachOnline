// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "AbilitySystemComponent.h"
#include "AbilityBase.h"

UAbilitySystemComponent::UAbilitySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();

	for (const auto& AbilityInfo : Abilities)
	{
		const FName AbilityName = AbilityInfo.Class.GetDefaultObject()->GetName();
		checkf(!AbilityObjects.Contains(AbilityName), TEXT("Same ability added twice"));

		auto Ability = NewObject<UAbilityBase>(this, AbilityInfo.Class, AbilityName);
		AbilityObjects.Add(AbilityName, Ability);

		Ability->Initialize(GetOwner(), AbilityInfo.IndicatorType, AbilityInfo.Consumption, AbilityInfo.Cooldown, AbilityInfo.ChargesNum);
	}
}

bool UAbilitySystemComponent::ActivateAbility(const FName& AbilityName)
{
	UAbilityBase* Ability = AbilityObjects.FindRef(AbilityName);
	if (Ability && Ability->IsActive())
	{
		Ability->Activate();
		return true;
	}
	return false;
}

bool UAbilitySystemComponent::ActivateAbilityWithParam(const FName& AbilityName, const FAbilityParam& Param)
{
	UAbilityBase* Ability = AbilityObjects.FindRef(AbilityName);
	if (Ability && Ability->IsActive())
	{
		Ability->ActivateWithParam(Param);
		return true;
	}
	return false;
}

bool UAbilitySystemComponent::HasAbility(const FName& AbilityName)
{
	return AbilityObjects.Contains(AbilityName);
}