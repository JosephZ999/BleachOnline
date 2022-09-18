// Authors MoonDi & JosephZzz for Bleach Online fan game

#include "BOAbilitySystemComponent.h"
#include "BOCharacterBase.h"
#include "BOAbilityBase.h"

UBOAbilitySystemComponent::UBOAbilitySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBOAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();

	for (const auto& AbilityInfo : Abilities)
	{
		const FName AbilityName = AbilityInfo.Class.GetDefaultObject()->GetName();
		checkf(! AbilityObjects.Contains(AbilityName), TEXT("Same ability added twice"));

		auto Ability = NewObject<UBOAbilityBase>(this, AbilityInfo.Class, AbilityName);
		AbilityObjects.Add(AbilityName, Ability);

		auto OwnerChar = Cast<ABOCharacterBase>(GetOwner());
		checkf(OwnerChar, TEXT("OwnerChar is null"));

		Ability->Initialize(OwnerChar, AbilityInfo.IndicatorType, AbilityInfo.Consumption, AbilityInfo.Cooldown, AbilityInfo.ChargesNum);
	}
}

bool UBOAbilitySystemComponent::ActivateAbility(const FName& AbilityName)
{
	UBOAbilityBase* Ability = AbilityObjects.FindRef(AbilityName);
	if (Ability && Ability->IsActive())
	{
		Ability->Activate();
		return true;
	}
	return false;
}

bool UBOAbilitySystemComponent::ActivateAbilityWithParam(const FName& AbilityName, const FAbilityParam& Param)
{
	UBOAbilityBase* Ability = AbilityObjects.FindRef(AbilityName);
	if (Ability && Ability->IsActive())
	{
		Ability->ActivateWithParam(Param);
		return true;
	}
	return false;
}

bool UBOAbilitySystemComponent::HasAbility(const FName& AbilityName)
{
	return AbilityObjects.Contains(AbilityName);
}
