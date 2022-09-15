// Authors MoonDi & JosephZzz for Bleach Online fan game

#include "BOAbilitySystemComponent.h"
#include "BOCharacterBase.h"

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
