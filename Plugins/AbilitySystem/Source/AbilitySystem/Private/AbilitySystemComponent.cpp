// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "AbilitySystemComponent.h"

UAbilitySystemComponent::UAbilitySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();
}
